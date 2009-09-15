#include "../include/config.h"
#include "../include/utils.h"
#include "../include/GameOptions.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <JGE.h>
#include <JFileSystem.h>
#include "../include/WResourceManager.h"

WResourceManager resources;

unsigned int vTime = 0;

//WResourceManager
void WResourceManager::DebugRender(){
  JRenderer* renderer = JRenderer::GetInstance();
  JLBFont * font = resources.GetJLBFont(Constants::MAIN_FONT);
  
  if(!font || !renderer)
    return;
  
  font->SetScale(DEFAULT_MAIN_FONT_SCALE);
  renderer->FillRect(0,0,SCREEN_WIDTH,20,ARGB(128,155,0,0));
#ifdef DEBUG_CACHE
  if(debugMessage.size())
    renderer->FillRect(0,SCREEN_HEIGHT-30,SCREEN_WIDTH,40,ARGB(128,155,0,0));
  else
#endif
    renderer->FillRect(0,SCREEN_HEIGHT-20,SCREEN_WIDTH,40,ARGB(128,155,0,0));
  char buf[512];

  
  unsigned long man = 0;
  unsigned int misses = 0;

  if(textureWCache.cacheItems < textureWCache.cache.size())
    misses = textureWCache.cache.size()-textureWCache.cacheItems;

  if(textureWCache.totalSize > textureWCache.cacheSize)
    man = textureWCache.totalSize - textureWCache.cacheSize;

  sprintf(buf,"Textures %u+%u (of %u) items (%u misses), Pixels: %lu+%lu (of %lu)",
    textureWCache.cacheItems, textureWCache.managed.size(),textureWCache.maxCached,
    misses,textureWCache.cacheSize,man,textureWCache.maxCacheSize);
  font->DrawString(buf, 10,5);



  sprintf(buf,"Total Size: %lu (%lu cached, %lu managed)",Size(),SizeCached(),SizeManaged());
  font->DrawString(buf, SCREEN_WIDTH-10,SCREEN_HEIGHT-15,JGETEXT_RIGHT);

#ifdef DEBUG_CACHE
  if(debugMessage.size())
    font->DrawString(debugMessage.c_str(), SCREEN_WIDTH-10,SCREEN_HEIGHT-25,JGETEXT_RIGHT);
#endif
}

unsigned long WResourceManager::Size(){
  unsigned long res = 0;
  res += textureWCache.totalSize;
  res += sampleWCache.totalSize;
  res += psiWCache.totalSize;
  return res;
}

unsigned long WResourceManager::SizeCached(){
  unsigned long res = 0;
  res += textureWCache.cacheSize;
  res += sampleWCache.cacheSize;
  res += psiWCache.cacheSize;
  return res;  
}

unsigned long WResourceManager::SizeManaged(){
  unsigned long res = 0;
  if(textureWCache.totalSize > textureWCache.cacheSize)
    res += textureWCache.totalSize - textureWCache.cacheSize;

  if(sampleWCache.totalSize > sampleWCache.cacheSize)
    res += sampleWCache.totalSize - sampleWCache.cacheSize;

  if(psiWCache.totalSize > psiWCache.cacheSize)
    res += psiWCache.totalSize - psiWCache.cacheSize;

  return res;  
}
unsigned int WResourceManager::Count(){
  unsigned int count = 0;
  count += textureWCache.cacheItems;
  count += textureWCache.managed.size();
  count += sampleWCache.cacheItems;
  count += sampleWCache.managed.size();
  count += psiWCache.cacheItems;
  count += psiWCache.managed.size();
  return count;
}
unsigned int WResourceManager::CountCached(){
  unsigned int count = 0;
  count += textureWCache.cacheItems;
  count += sampleWCache.cacheItems;
  count += psiWCache.cacheItems;
  return count;
}
unsigned int WResourceManager::CountManaged(){
  unsigned int count = 0;
  count += textureWCache.managed.size();
  count += sampleWCache.managed.size();
  count += psiWCache.managed.size();
  return count;
}

unsigned int WResourceManager::nowTime(){
  if(lastTime == 65535)
    FlattenTimes();
  
  return ++lastTime;
}

void WResourceManager::FlattenTimes(){
  unsigned int t;
  lastTime = sampleWCache.Flatten();  

  t = textureWCache.Flatten();
  if(t > lastTime)
    lastTime = t;

  t = psiWCache.Flatten();
  if(t > lastTime)
    lastTime = t;
}

WResourceManager::WResourceManager(){	

  #ifdef WIN32
  char buf [4096];
  sprintf(buf, " Init WResourceManager : %p\n", this);
  OutputDebugString(buf);
  #endif
#ifdef DEBUG_CACHE
  menuCached = 0;
#endif
  mTextureList.clear();
	mTextureList.reserve(0);
	mTextureMap.clear();

	mQuadList.clear();
	mQuadList.reserve(0);
	mQuadMap.clear();

	mFontList.clear();
	mFontList.reserve(4);
	mFontMap.clear();

  psiWCache.Resize(SMALL_CACHE_LIMIT,6);      //Plenty of room for mana symbols, or whatever.
  sampleWCache.Resize(SMALL_CACHE_LIMIT,0);   //This guy only exists so we can track misses.
  textureWCache.Resize(LARGE_CACHE_LIMIT,MAX_CACHE_OBJECTS);
  lastTime = 0;
}
WResourceManager::~WResourceManager(){
  LOG("==Destroying WResourceManager==");
  RemoveAll();
  LOG("==Successfully Destroyed WResourceManager==");
}

JQuad * WResourceManager::RetrieveCard(MTGCard * card, int style, int submode){
  //Cards are never, ever resource managed, so just check cache.

  submode = submode | TEXTURE_SUB_CARD;

  string filename = card->getSetName();
  filename += "/";
  filename += card->getImageName();
  WCachedTexture * res = NULL;

  //Aliases.
  if(style == RETRIEVE_VRAM){
    submode = submode | TEXTURE_SUB_VRAM;
    style = RETRIEVE_LOCK;
  } 
  else if(style == RETRIEVE_THUMB){
    submode = submode | TEXTURE_SUB_THUMB;
    style = RETRIEVE_NORMAL;
  }

  res = textureWCache.Retrieve(filename,style,submode|TEXTURE_SUB_5551);  //Force RGBA5650. JPG doesn't support transparency anyways.

  if(res) //A non-null result will always be good.
    return res->GetCard();

  return NULL;
}

int WResourceManager::CreateQuad(const string &quadName, const string &textureName, float x, float y, float width, float height){
  if(!quadName.size() || !textureName.size())
    return INVALID_ID;
  
  string resname = quadName;
  std::transform(resname.begin(),resname.end(),resname.begin(),::tolower);

  vector<WManagedQuad>::iterator it;
  int pos = 0;
  for(it = managedQuads.begin();it!=managedQuads.end();it++,pos++){
    if(it->resname == resname)
      return pos;
  }

  WCachedTexture * jtex = textureWCache.Retrieve(textureName,RETRIEVE_MANAGE);
  
  //Somehow, jtex wasn't promoted.
  if(RETRIEVE_MANAGE && jtex && !jtex->isPermanent())
    return INVALID_ID; 

  if(jtex){
     JQuad * jq = jtex->GetQuad(x,y,width,height,quadName);  
     
    if(jq){
      WManagedQuad mq;
      mq.resname = resname;
      mq.texture = jtex;
      managedQuads.push_back(mq);
    }

    return (int) (managedQuads.size() - 1);
  }

  return INVALID_ID;
}

JQuad * WResourceManager::GetQuad(const string &quadName){
  string lookup = quadName;
  std::transform(lookup.begin(),lookup.end(),lookup.begin(),::tolower);
  
  for(vector<WManagedQuad>::iterator it=managedQuads.begin();it!=managedQuads.end();it++){
    if(it->resname == lookup)
      return it->texture->GetQuad(lookup);
  }

  return NULL;
}

JQuad * WResourceManager::GetQuad(int id){
  if(id < 0 || id >= (int) managedQuads.size())
    return NULL;

  WCachedTexture * jtex = managedQuads[id].texture;
  if(!jtex)
    return NULL;

  return jtex->GetQuad(managedQuads[id].resname);
}

JQuad * WResourceManager::RetrieveTempQuad(string filename){
  return RetrieveQuad(filename,0,0,0,0,"",RETRIEVE_NORMAL);
}

JQuad * WResourceManager::RetrieveQuad(string filename, float offX, float offY, float width, float height, string resname, int style, int submode){
  JQuad * jq = NULL;

  //Lookup managed resources, but only with a real resname.
  if(resname.size() && (style == RETRIEVE_MANAGE || style == RETRIEVE_RESOURCE)){
   jq = GetQuad(resname); 
   if(jq || style == RETRIEVE_RESOURCE)
     return jq;
  }  

  //Resname defaults to filename.
  if(!resname.size())
    resname = filename;

  //No quad, but we have a managed texture for this!
  WCachedTexture * jtex = textureWCache.Retrieve(filename,style,submode);

  //Somehow, jtex wasn't promoted.
  if(style == RETRIEVE_MANAGE && jtex && !jtex->isPermanent())
    return NULL;  

  //Make this quad, overwriting any similarly resname'd quads.
  if(jtex){
     jq = jtex->GetQuad(offX,offY,width,height,resname);  
     
    if(style == RETRIEVE_MANAGE && resname != ""){
      WManagedQuad mq;
      mq.resname = resname;
      mq.texture = jtex;
      managedQuads.push_back(mq);
    }

    return jq;
  }

  //Texture doesn't exist, so no quad.
  return NULL;
}
void WResourceManager::Release(JTexture * tex){
  if(!tex)
    return;

  textureWCache.Release(tex);
}

void WResourceManager::Release(JQuad * quad){
  if(!quad)
    return;

  map<string,WCachedTexture*>::iterator it;
  for(it = textureWCache.cache.begin();it!=textureWCache.cache.end();it++){
    if(it->second && it->second->ReleaseQuad(quad))
      break;
  }

  if(it != textureWCache.cache.end() && it->second)
    textureWCache.RemoveItem(it->second,false); //won't remove locked.
}

void WResourceManager::ClearMisses(){
  textureWCache.ClearMisses();
  sampleWCache.ClearMisses();
  psiWCache.ClearMisses();
}

void WResourceManager::ClearUnlocked(){
  textureWCache.ClearUnlocked();
  sampleWCache.ClearUnlocked();
  psiWCache.ClearUnlocked();
}

bool WResourceManager::Cleanup(){
  int check = 0;

  if(textureWCache.Cleanup())
    check++;
  if(sampleWCache.Cleanup())
    check++;
  if(psiWCache.Cleanup())
    check++;

  return (check > 0);
}
void WResourceManager::Release(JSample * sample){
  if(!sample)
    return;

  sampleWCache.Release(sample);
}

JTexture * WResourceManager::RetrieveTexture(string filename, int style, int submode){
  WCachedTexture * res = NULL;

  //Aliases.
  if(style == RETRIEVE_VRAM){
    submode = submode | TEXTURE_SUB_VRAM;
    style = RETRIEVE_LOCK;
  } 
  else if(style == RETRIEVE_THUMB){
    submode = submode | TEXTURE_SUB_THUMB;
    style = RETRIEVE_NORMAL;
  }

  res = textureWCache.Retrieve(filename,style,submode);

  if(res){ //a non-null result will always be good.
    JTexture * t = res->Actual();
    JRenderer::GetInstance()->BindTexture(t);
    return t;
  }
#ifdef DEBUG_CACHE
  else{
    switch(textureWCache.mError){
      case CACHE_ERROR_NONE:
        debugMessage = "Not in cache: ";
        break;
      case CACHE_ERROR_404:
        debugMessage = "File not found: ";
        break;
      case CACHE_ERROR_BAD:
        debugMessage = "Cache bad: ";
        break;
      case CACHE_ERROR_NOT_MANAGED:
        debugMessage = "Resource not managed: ";
        break;
      case CACHE_ERROR_LOST:
        debugMessage = "Resource went bad, potential memory leak: ";
        break;
      default:
        debugMessage = "Unspecified error: ";
    }
    debugMessage += filename;
  }
#endif

  return NULL;
}

int WResourceManager::CreateTexture(const string &textureName) {
  JTexture * jtex = RetrieveTexture(textureName,RETRIEVE_MANAGE);
  
  if(jtex)
    return (int) jtex->mTexId;  //Because it's unsigned on windows/linux.

  return INVALID_ID;
}

JTexture* WResourceManager::GetTexture(const string &textureName){
  JTexture * jtex = RetrieveTexture(textureName,RETRIEVE_RESOURCE);
  return jtex;
}

JTexture* WResourceManager::GetTexture(int id){
  map<string,WCachedTexture*>::iterator it;
  JTexture *jtex = NULL;

  if(id == INVALID_ID)
    return NULL;

  for(it = textureWCache.managed.begin();it!= textureWCache.managed.end(); it++){
    if(it->second){
      jtex = it->second->Actual();
      if(id == (int) jtex->mTexId)
        return jtex;
    }
  }

  return jtex;
}

hgeParticleSystemInfo * WResourceManager::RetrievePSI(string filename, JQuad * texture, int style, int submode){
  
  if(!texture)
    return NULL;

  WCachedParticles * res = psiWCache.Retrieve(filename,style,submode);

  if(res) //A non-null result will always be good.
  {
    hgeParticleSystemInfo * i = res->Actual();
    i->sprite = texture;
    return i;
  }

  return NULL;
}

JSample * WResourceManager::RetrieveSample(string filename, int style, int submode){
  //Check cache. This just tracks misses.
  WCachedSample * tc = NULL;
  tc = sampleWCache.Get(filename,submode);

  //Sample exists! Get it.
  if(tc && tc->isGood()){
    JSample * js = tc->Actual();

    //Samples are freed when played, so remove this. Because maxCached is 0, this will Nullify() first.
    sampleWCache.RemoveItem(tc,true);
    //Adjust sizes accordingly.
    sampleWCache.cacheSize = 0;
    sampleWCache.totalSize = 0;
    return js;  
  }

  return NULL;
}

string WResourceManager::graphicsFile(const string filename, const string specific){
    char buf[512];
   
    //Check the specific location, if any.
    if(specific != ""){
      sprintf(buf,"%s/%s",specific.c_str(),filename.c_str());
      if(fileOK(buf,true))
        return buf;
    }

    //Check the theme folder.
    string theme = options[Options::ACTIVE_THEME].str;
    std::transform(theme.begin(), theme.end(), theme.begin(), ::tolower);

    if(theme != "" && theme != "default"){
      sprintf(buf,"themes/%s/%s",theme.c_str(),filename.c_str());
     if(fileOK(buf,true))
        return buf;
    }

    //Failure. Check mode graphics     
    string mode = options[Options::ACTIVE_MODE].str;
    std::transform(mode.begin(), mode.end(), mode.begin(), ::tolower);

    if(mode != "" && mode != "defualt"){
      sprintf(buf,"modes/%s/graphics/%s",mode.c_str(),filename.c_str());
      if(fileOK(buf,true))
        return buf;
    }
       
     //Failure. Check graphics       
     char graphdir[512];
     sprintf(graphdir,"graphics/%s",filename.c_str());
      if(fileOK(graphdir,true))
        return graphdir;
    
     //Failure. Check sets.       
     sprintf(buf,"sets/%s",filename.c_str());
      if(fileOK(buf,true))
        return buf;

     //Complete abject failure. Probably a crash...
     return graphdir;
}


string WResourceManager::avatarFile(const string filename, const string specific){
    char buf[512];
   
    //Check the specific location, if any.
    if(specific != ""){
      sprintf(buf,"%s/%s",specific.c_str(),filename.c_str());
      if(fileOK(buf,true))
        return buf;
    }
  
    //Check the profile folder.
    string profile = options[Options::ACTIVE_PROFILE].str;
    std::transform(profile.begin(), profile.end(), profile.begin(), ::tolower);
    if(profile != "" && profile != "default"){
      sprintf(buf,"profiles/%s/%s",profile.c_str(),filename.c_str());
     if(fileOK(buf,true))
        return buf;
    }else{
      sprintf(buf,"player/%s",filename.c_str());
     if(fileOK(buf,true))
        return buf;
    }

    //Check the theme folder.
    string theme = options[Options::ACTIVE_THEME].str;
    std::transform(theme.begin(), theme.end(), theme.begin(), ::tolower);

    if(theme != "" && theme != "default"){
      sprintf(buf,"themes/%s/%s",theme.c_str(),filename.c_str());
     if(fileOK(buf,true))
        return buf;
    }

    //Failure. Check mode graphics     
    string mode = options[Options::ACTIVE_MODE].str;
    std::transform(mode.begin(), mode.end(), mode.begin(), ::tolower);

    if(mode != "" && mode != "defualt"){
      sprintf(buf,"modes/%s/graphics/%s",mode.c_str(),filename.c_str());
      if(fileOK(buf,true))
        return buf;
    }

    //Failure. Check Baka
    sprintf(buf,"ai/baka/avatars/%s",filename.c_str());
     if(fileOK(buf,true))
        return buf;
       
     //Failure. Check graphics       
     char graphdir[512];
     sprintf(graphdir,"graphics/%s",filename.c_str());
      if(fileOK(graphdir,true))
        return graphdir;
    
     //Complete abject failure. Probably a crash...
     return graphdir;
}

string WResourceManager::cardFile(const string filename, const string specific){
    JFileSystem* fs = JFileSystem::GetInstance();
    char buf[512];

    fs->DetachZipFile();

    //Check the specific location, if any.
    if(specific != ""){
      sprintf(buf,"%s/sets/%s",specific.c_str(),filename.c_str());
      if(fileOK(buf,true))
        return buf;
    }

    //Check the theme folder.
    string theme = options[Options::ACTIVE_THEME].str;
    std::transform(theme.begin(), theme.end(), theme.begin(), ::tolower);

    if(theme != "" && theme != "default"){
       sprintf(buf,"themes/%s/sets/%s",theme.c_str(),filename.c_str());
       if(fileOK(buf,true))
         return buf;
    }

    //Failure. Check mode     
    string mode = options[Options::ACTIVE_MODE].str;
    std::transform(mode.begin(), mode.end(), mode.begin(), ::tolower);

    if(mode != "" && mode != "defualt"){
      sprintf(buf,"modes/%s/sets/%s",mode.c_str(),filename.c_str());
    if(fileOK(buf,true))
       return buf;      
    }
       
     //Failure. Check sets       
     char defdir[512];
     sprintf(defdir,"sets/%s",filename.c_str());
     if(fileOK(defdir,true))
       return defdir;      

     //Failure. Assume it's in a zip file?
     string::size_type i;
     for(i = 0;i < filename.size();i++){
      if(filename[i] == '\\' || filename[i] == '/')
          break;
     } 

     if(i != filename.size()){
       string set = filename.substr(0,i);

       if(set.size()){
          char zipname[512];
          sprintf(zipname, "Res/sets/%s/%s.zip", set.c_str(),set.c_str());
          if (fileOK(zipname)){
            fs->AttachZipFile(zipname);
            return filename.substr(i+1);
          }
       }
     }

     //Complete failure.
     return "";
}

string WResourceManager::musicFile(const string filename, const string specific){
    char buf[512];

    //Check the specific location, if any.
    if(specific != ""){
      sprintf(buf,"%s/%s",specific.c_str(),filename.c_str());
      if(fileOK(buf,true))
        return buf;
    }

    //Check the theme folder.
    string theme = options[Options::ACTIVE_THEME].str;
    std::transform(theme.begin(), theme.end(), theme.begin(), ::tolower);

    if(theme != "" && theme != "default"){
       sprintf(buf,"themes/%s/sound/%s",theme.c_str(),filename.c_str());
       if(fileOK(buf,true))
         return buf;
    }

    //Failure. Check mode     
    string mode = options[Options::ACTIVE_MODE].str;
    std::transform(mode.begin(), mode.end(), mode.begin(), ::tolower);

    if(mode != "" && mode != "defualt"){
      sprintf(buf,"modes/%s/sound/%s",mode.c_str(),filename.c_str());
    if(fileOK(buf,true))
       return buf;      
    }
       
     //Failure. Check sound       
     char defdir[512];
     sprintf(defdir,"sound/%s",filename.c_str());
     if(fileOK(defdir,true))
       return defdir;      
    
     //Complete abject failure. Probably a crash...
     return defdir;
}

string WResourceManager::sfxFile(const string filename, const string specific){
    char buf[512];

    //Check the specific location, if any.
    if(specific != ""){
      sprintf(buf,"%s/%s",specific.c_str(),filename.c_str());
      if(fileOK(buf,true))
        return buf;
    }

    //Check the theme folder.
    string theme = options[Options::ACTIVE_THEME].str;
    std::transform(theme.begin(), theme.end(), theme.begin(), ::tolower);

    if(theme != "" && theme != "default"){
       sprintf(buf,"themes/%s/sound/sfx/%s",theme.c_str(),filename.c_str());
       if(fileOK(buf,true))
         return buf;
    }

    //Failure. Check mode     
    string mode = options[Options::ACTIVE_MODE].str;
    std::transform(mode.begin(), mode.end(), mode.begin(), ::tolower);
    if(mode != "" && mode != "defualt"){
      sprintf(buf,"modes/%s/sound/sfx/%s",mode.c_str(),filename.c_str());
    if(fileOK(buf,true))
       return buf;      
    }
       
     //Failure. Check sound       
     char defdir[512];
     sprintf(defdir,"sound/sfx/%s",filename.c_str());
     if(fileOK(defdir,true))
       return defdir;      
    
     //Complete abject failure. Probably a crash...
     return "";
}

int WResourceManager::fileOK(string filename, bool relative){
  char fname[512];
  std::ifstream * fp = NULL;
  if(relative){
    sprintf(fname,RESPATH"/%s",filename.c_str());
    fp = NEW std::ifstream(fname);
  }
  else
    fp = NEW std::ifstream(filename.c_str());

  int result = 0;
  if(fp){
    if(*fp) result = 1;
    fp->close();
    delete fp;
  }

  return result;
}

int WResourceManager::LoadJLBFont(const string &fontName, int height){
  map<string, int>::iterator itr = mFontMap.find(fontName);

	if (itr == mFontMap.end())
	{
		string path = graphicsFile(fontName);

		printf("creating font:%s\n", path.c_str());

		int id = mFontList.size();
		mFontList.push_back(NEW JLBFont(path.c_str(), height, true));

		mFontMap[fontName] = id;

		return id;
	}
	else
		return itr->second;
}
void WResourceManager::LargeCache(){
#if (defined WIN32 || defined LINUX) && !defined DEBUG_CACHE
  return;
#else 
    //Does not check option so we can use large cache during 
    //deck editor, when it's pretty much always safe.
    textureWCache.Resize(LARGE_CACHE_LIMIT,MAX_CACHE_OBJECTS);
    textureWCache.Cleanup();
#endif
}
void WResourceManager::SmallCache(){
#if (defined WIN32 || defined LINUX) && !defined DEBUG_CACHE
  return;
#else
  if(options[Options::CACHESIZE].number == 0){
    textureWCache.Resize(SMALL_CACHE_LIMIT,MAX_CACHE_OBJECTS);
    textureWCache.Cleanup();}
#endif
}

JMusic * WResourceManager::ssLoadMusic(const char *fileName){
  return JSoundSystem::GetInstance()->LoadMusic(musicFile(fileName).c_str());
}


void WResourceManager::Refresh(){
  //Really easy cache relinking.
  sampleWCache.Refresh();
  textureWCache.Refresh();
  psiWCache.Refresh();

  map<string,WCachedTexture*>::iterator it;
  vector<JQuad*>::iterator q;

  //Now do some juggling so that managed resources also reload. 
  map<JTexture *,JTexture *> oldTextures;
  map<JTexture *,string> newNames;
  map<JTexture *,JTexture *>::iterator oldIt;
  vector<JTexture*>::iterator jtex;
  map<string, int>::iterator mapping;
  JTexture * newtex;
  JTexture * oldtex = NULL;

  //Store old mappings.
  for(mapping = mTextureMap.begin();mapping != mTextureMap.end();mapping++){
    if(oldTextures[mTextureList[mapping->second]] == NULL){
      newtex = JRenderer::GetInstance()->LoadTexture(graphicsFile(mapping->first).c_str(),0,TEXTURE_FORMAT);
      oldtex = mTextureList[mapping->second];
      if(!newtex)
        newNames[oldtex] = mapping->first;
      else{
        newNames[newtex] = mapping->first;
        JRenderer::GetInstance()->BindTexture(newtex);
      }

      oldTextures[oldtex] = newtex;
    }
  }

  //Remap quads.
  for(q = mQuadList.begin();q!=mQuadList.end();q++){
    newtex = oldTextures[(*q)->mTex];
    if(newtex != NULL)
      (*q)->mTex = newtex;
  }

  //Rebuild mTextureList and mapping.
  mTextureList.clear();
  mTextureMap.clear();
  int x = 0;
  for(oldIt = oldTextures.begin();oldIt!=oldTextures.end();oldIt++){
    
    if(oldIt->second)
      newtex = oldIt->second;
    else
      newtex = oldIt->first;
    
    mTextureList.push_back(newtex);
    mTextureMap[newNames[newtex]] = x;
    x++;
  }
  
  //Rebuild mapping.
  for(mapping = mTextureMap.begin();mapping != mTextureMap.end();mapping++){
    if(oldTextures[mTextureList[mapping->second]] == NULL)
      continue;
  }

  //Delete unused textures.
  for(oldIt = oldTextures.begin();oldIt!=oldTextures.end();oldIt++){
    if(!oldIt->second || !oldIt->first )
      continue;

    SAFE_DELETE(oldtex);
  }
}

//WCache
template <class cacheItem,class cacheActual> 
bool WCache<cacheItem, cacheActual>::RemoveOldest(){
  typename map<string,cacheItem*> ::iterator oldest = cache.end();

  for(typename map<string,cacheItem *>::iterator it = cache.begin();it!=cache.end();it++){
    if(it->second && !it->second->isLocked() 
      && (oldest == cache.end() || it->second->lastTime < oldest->second->lastTime))
      oldest = it;
  }

  if(oldest != cache.end() && oldest->second && !oldest->second->isLocked()){
    unsigned long isize = oldest->second->size();
    cacheSize -= isize;
    totalSize -= isize;
    cacheItems--;
#if defined DEBUG_CACHE
    lastExpired = oldest->first;
#endif
    Delete(oldest->second);
    cache.erase(oldest);
    return true;
  }

  return false;

}
template <class cacheItem, class cacheActual>
void WCache<cacheItem, cacheActual>::Clear(){
  typename map<string,cacheItem*>::iterator it, next;

  for(it = cache.begin(); it != cache.end();it=next){
    next = it;
    next++;

      if(it->second){
        unsigned long isize = it->second->size();
        totalSize -= isize;
        cacheSize -= isize;
        Delete(it->second);
        cacheItems--;
      }
      cache.erase(it);
  }
  for(it = managed.begin(); it != managed.end();it=next){
    next = it;
    next++;

    if(!it->second)
      managed.erase(it);
  }
}

template <class cacheItem, class cacheActual>
void WCache<cacheItem, cacheActual>::ClearUnlocked(){
  typename map<string,cacheItem*>::iterator it, next;

  for(it = cache.begin(); it != cache.end();it=next){
    next = it;
    next++;

      if(it->second && !it->second->isLocked()){
        unsigned long isize = it->second->size();
        totalSize -= isize;
        cacheSize -= isize;
        Delete(it->second);
        cacheItems--;
        cache.erase(it);
      }
      else if(!it->second){
        cache.erase(it);
      }
  }
}

template <class cacheItem, class cacheActual>
void WCache<cacheItem, cacheActual>::ClearMisses(){
  typename map<string,cacheItem*>::iterator it, next;

  for(it = cache.begin(); it != cache.end();it=next){
    next = it;
    next++;

    if(!it->second)
      cache.erase(it);
  }
  for(it = managed.begin(); it != managed.end();it=next){
    next = it;
    next++;

    if(!it->second)
      managed.erase(it);
  }
}
template <class cacheItem, class cacheActual>
void WCache<cacheItem, cacheActual>::Resize(unsigned long size, int items){
    maxCacheSize = size;

  if(items > MAX_CACHE_OBJECTS || items < 0)
    maxCached = MAX_CACHE_OBJECTS;
  else
    maxCached = items;
}
template <class cacheItem, class cacheActual>
cacheItem* WCache<cacheItem, cacheActual>::AttemptNew(string filename, int submode){
//  Cleanup();  

  if(submode & CACHE_EXISTING){ //Should never get this far.
    mError = CACHE_ERROR_NOT_CACHED;
    return NULL;
  }

  cacheItem* item = NEW cacheItem;

  mError = CACHE_ERROR_NONE;

  if(item == NULL || !item->Attempt(filename,submode,mError)){
    //No such file. Fail.
    if(mError == CACHE_ERROR_404){
      Delete(item);
      return NULL;
    }


    for(int attempt=0;attempt<10;attempt++){
      if(!RemoveOldest()) 
        break;
      
      if(!item)
        item = NEW cacheItem ;

      if(item && item->Attempt(filename,submode,mError))
        break;
    }


    //Still no result, so clear cache entirely, then try again.
    if(!item || !item->isGood()){
      ClearUnlocked();
      if(!item) item = NEW(cacheItem);
      item->Attempt(filename,submode,mError);
    }

    //Worst cache scenerio. Clear every cache we've got.
    if(!item || !item->isGood()){
      resources.ClearUnlocked();
      if(!item) item = NEW(cacheItem);
      item->Attempt(filename,submode,mError);
    }
  }

  if(item && !item->isGood()){
    Delete(item);
    mError = CACHE_ERROR_BAD;
  }
  else
    mError = CACHE_ERROR_NONE;
 
  Cleanup();  
  return item;
}

template <class cacheItem, class cacheActual>
cacheItem * WCache<cacheItem, cacheActual>::Retrieve(string filename, int style, int submode){
  //Check cache.
  cacheItem * tc = NULL;

  if(style == RETRIEVE_EXISTING || style == RETRIEVE_RESOURCE)
    tc = Get(filename,style,submode|CACHE_EXISTING);
  else
    tc = Get(filename,style,submode);

  //Retrieve resource only works on permanent items.
  if(style == RETRIEVE_RESOURCE && tc && !tc->isPermanent()){
    mError = CACHE_ERROR_NOT_MANAGED;
    return NULL;
  }
  
  //Perform lock or unlock on entry.
  if(tc){
    if(style == RETRIEVE_LOCK) tc->lock();
    else if(style == RETRIEVE_UNLOCK) tc->unlock();
    else if(style == RETRIEVE_MANAGE && !tc->isPermanent()) {
      //Unlink the managed resource from the cache.
      UnlinkCache(tc);
      
      //Post it in managed resources.
      managed[makeID(filename,submode)] = tc;
      tc->deadbolt();
    }
  }

  //Resource exists! 
  if(tc){
    if(tc->isGood()){
      tc->hit();    
      return tc;  //Everything fine.
    }
    else{
      //Something went wrong.
      Delete(tc);
    }
  }

  //Record managed failure. Cache failure is recorded in Get().
  if(style == RETRIEVE_MANAGE || style == RETRIEVE_RESOURCE)
   managed[makeID(filename,submode)] = NULL; 

  return NULL;
}
template <class cacheItem, class cacheActual>
string WCache<cacheItem, cacheActual>::makeID(string id, int submode){
  string lookup = id;
  
  //To differentiate between cached thumbnails and the real thing.
  if(submode & TEXTURE_SUB_THUMB)
    lookup.insert(0,"T");
  
  return lookup;
}

template <class cacheItem, class cacheActual>
string WCache<cacheItem, cacheActual>::makeFilename(string id, int submode){
  //To differentiate between cached thumbnails and the real thing.
  if(submode & TEXTURE_SUB_THUMB)
    return id.substr(1);
  
  return id;
}

template <class cacheItem, class cacheActual>
cacheItem * WCache<cacheItem, cacheActual>::Get(string id, int style, int submode){
  typename map<string,cacheItem*>::iterator it;
  string lookup = makeID(id,submode);  

  //Check for managed resources first. Always
  it = managed.end();
  for(it = managed.begin();it!=managed.end();it++){
    if(it->first == lookup)
      break;
  }  
  //Something is managed.
  if(it != managed.end()) {
     if(!it->second && style == RETRIEVE_RESOURCE)
        return NULL;     //A miss. 
     else
        return it->second; //A hit.
  }
  //Failed to find managed resource and won't create one. Record a miss.
  else if(style == RETRIEVE_RESOURCE){
    managed[lookup] = NULL;
    return NULL;
  }

  //Not managed, so look in cache.
  if(it == managed.end() && style != RETRIEVE_MANAGE && style != RETRIEVE_RESOURCE ){
    it = cache.end();
    for(it = cache.begin();it!=cache.end();it++){
      if(it->first == lookup)
        break;
    }
    //Well, we've found something...
    if(it != cache.end()) {
       if(!it->second && (submode & CACHE_EXISTING))
         return NULL;     //A miss.
       else
         return it->second; //A hit.
    }
  }  

  cacheItem * item = NULL;
  
  if(style != RETRIEVE_MANAGE)
    item = cache[lookup];   //We don't know about this one yet.
 
  //Found something.
  if(item){
     //Item went bad?
    if(!item->isGood()){

      //If we're allowed, attempt to revive it.
      if(!(submode & CACHE_EXISTING))
        item->Attempt(id,submode,mError);

      //Still bad, so remove it and return NULL 
      if(submode & CACHE_EXISTING || !item->isGood()){
        if(!item->isLocked()){
          RemoveItem(item);    //Delete it.
          mError = CACHE_ERROR_BAD;
        }
        //Worst case scenerio. Hopefully never happens.... hasn't so far.
        else{
          item->Nullify();          //We're giving up on anything allocated here. 
          mError = CACHE_ERROR_LOST; //This is a potential memory leak, but might prevent a crash. 
        }
        return NULL;
      }
    }

    //Alright, everythings fine!
    mError = CACHE_ERROR_NONE;  
    return item;
  }
  
  //Didn't exist in cache.
  if(submode & CACHE_EXISTING ){  
    RemoveMiss(lookup);
    mError = CACHE_ERROR_NOT_CACHED;
    return NULL;
  }
  else{
    //Space in cache, make new texture
     item = AttemptNew(id,submode);       

     //Couldn't make new item.
     if(item && !item->isGood())
       Delete(item);
  }
    
   if(style == RETRIEVE_MANAGE){
     if(item){
      managed[lookup] = item; //Record a hit.
      item->deadbolt(); //Make permanent.
     }
     else if(mError == CACHE_ERROR_404)
      managed[lookup] = item;  //File not found. Record a miss
   }
   else{
    if(!item && mError != CACHE_ERROR_404)
     RemoveMiss(lookup);
    else
     cache[lookup] = item;
   }

   //Succeeded in making a new item.
   if(item){
    unsigned long isize =item->size();   
    totalSize += isize;
    
    mError = CACHE_ERROR_NONE;
    if(style != RETRIEVE_MANAGE){
      cacheItems++;
      cacheSize += isize;
    }

    return item;
  }

  //Failure.
  return NULL;
}

template <class cacheItem, class cacheActual>
void WCache<cacheItem, cacheActual>::Refresh(){
  typename map<string,cacheItem*>::iterator it;
  ClearUnlocked();

  for(it = cache.begin();it!=cache.end();it++){
    if(it->second){
      it->second->Refresh(makeFilename(it->first,it->second->loadedMode));
    }
  } 
  for(it = managed.begin();it!=managed.end();it++){
    if(it->second){
      it->second->Refresh(makeFilename(it->first,it->second->loadedMode));
    }
  }
}

template <class cacheItem, class cacheActual>
WCache<cacheItem, cacheActual>::WCache(){
  cacheSize = 0;
  totalSize = 0;

  maxCacheSize = SMALL_CACHE_LIMIT;

  maxCached = MAX_CACHE_OBJECTS;
  cacheItems = 0;
  mError = CACHE_ERROR_NONE;
}

template <class cacheItem, class cacheActual>
WCache<cacheItem, cacheActual>::~WCache(){
  typename map<string,cacheItem*>::iterator it;

  for(it=cache.begin();it!=cache.end();it++){
    if(!it->second)
      continue;

    Delete(it->second);
  }

  for(it=managed.begin();it!=managed.end();it++){
    if(!it->second)
      continue;

    Delete(it->second);
  }
}


template <class cacheItem, class cacheActual>
bool WCache<cacheItem, cacheActual>::Cleanup(){
  while(cacheItems < cache.size() && cache.size() - cacheItems > MAX_CACHE_MISSES){
    RemoveMiss();
  }

  while (cacheItems > MAX_CACHE_OBJECTS || cacheItems > maxCached || cacheSize > maxCacheSize ){
    if (!RemoveOldest()) 
      return false;
  } 
  return true;
}

template <class cacheItem, class cacheActual>
unsigned int WCache<cacheItem, cacheActual>::Flatten(){
  unsigned int youngest = 65535;
  unsigned int oldest = 0;

  for (typename map<string,cacheItem*>::iterator it = cache.begin(); it != cache.end(); ++it){
    if(!it->second) continue;
    if(it->second->lastTime < youngest) youngest = it->second->lastTime;
    if(it->second->lastTime > oldest) oldest = it->second->lastTime;
  }

  for (typename map<string,cacheItem*>::iterator it = cache.begin(); it != cache.end(); ++it){
    if(!it->second) continue;
      it->second->lastTime -= youngest;
  }

  return (oldest - youngest);
}

template <class cacheItem, class cacheActual>
bool WCache<cacheItem, cacheActual>::RemoveMiss(string id){
  typename map<string,cacheItem*>::iterator it = cache.end();

  for(it = cache.begin();it!=cache.end();it++){
    if((id == "" || it->first == id) && it->second == NULL)
          break;
  }

  if(it != cache.end())
  {
    cache.erase(it);
    return true; 
  }

  return false;
}

template <class cacheItem, class cacheActual>
bool WCache<cacheItem, cacheActual>::RemoveItem(cacheItem * item, bool force){
  typename map<string,cacheItem*>::iterator it = cache.end();

  if(item == NULL)
    return false;   //Use RemoveMiss to remove cache misses, not this.

  for(it = cache.begin();it!=cache.end();it++){
    if(it->second == item)
      break;
  }
  if(it != cache.end() && it->second && (force || !it->second->isLocked())){
    unsigned long isize = it->second->size();
    totalSize -= isize;
    cacheSize -= isize;

    cacheItems--;
#if defined DEBUG_CACHE
    lastRemoved = it->first;
#endif
    Delete(it->second);
    cache.erase(it);
    return true;
  }

  return false;
}

template <class cacheItem, class cacheActual>
bool WCache<cacheItem, cacheActual>::UnlinkCache(cacheItem * item){
  typename map<string,cacheItem*>::iterator it = cache.end();

  if(item == NULL)
    return false;   //Use RemoveMiss to remove cache misses, not this.

  for(it = cache.begin();it!=cache.end();it++){
    if(it->second == item)
      break;
  }
  if(it != cache.end() && it->second){
    it->second = NULL;  
    unsigned long isize = item->size();

    cacheSize-=isize; 
    cacheItems--;
    cache.erase(it);
    return true;
  }

  return false;
}

template <class cacheItem, class cacheActual>
bool WCache<cacheItem, cacheActual>::Delete(cacheItem * item){
  if(!item)
    return false;

  if(maxCached == 0)
    item->Nullify();

  SAFE_DELETE(item);
  return true;
}

template <class cacheItem, class cacheActual>
bool WCache<cacheItem, cacheActual>::Release(cacheActual* actual){
  if(!actual)
    return false;

  typename map<string,cacheItem*>::iterator it;
  for(it=cache.begin();it!=cache.end();it++){    
    if(it->second && it->second->compare(actual))
      break;
  }
  
  if(it == cache.end())
    return false; //Not here, can't release.

  if(it->second){
    it->second->unlock(); //Release one lock.
    if(it->second->isLocked())
      return true; //Still locked, won't delete.

    unsigned long isize = it->second->size();
    totalSize -= isize;
    cacheSize -= isize;
    cacheItems--;
#if defined DEBUG_CACHE
    lastReleased = it->first;
#endif
    Delete(it->second);
  }

  //Released!
  cache.erase(it);
  return true;
}

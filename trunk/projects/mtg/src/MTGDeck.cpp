#include "../include/config.h"
#include "../include/MTGDeck.h"
#include "../include/utils.h"
#include <algorithm>
#include <string>
using std::string;

#include <JGE.h>

#if defined (WIN32) || defined (LINUX)
#include <time.h>
#endif





MtgSets * MtgSets::SetsList = NEW MtgSets();



MtgSets::MtgSets(){
  nb_items = 0;
}

int MtgSets::Add(const char * name){
  string value = name;
  values[nb_items] = value;
  nb_items++;
  return nb_items - 1;
}

int MtgSets::find(string name){
  std::transform(name.begin(), name.end(), name.begin(),::tolower );
  for (int i = 0; i < nb_items; i++){
    string set = values[i];
    std::transform(set.begin(), set.end(), set.begin(),::tolower );;
    if (set.compare(name) == 0) return i;
  }
  return -1;
}


int MTGAllCards::processConfLine(string s, MTGCard *card){
  unsigned int i = s.find_first_of("=");
  if (i == string::npos) return 0;
  string key = s.substr(0,i);
  string value = s.substr(i+1);

  if(key.compare( "auto")==0){
    card->addMagicText(value);
  }
  else if(key.compare( "alias")==0){
    card->alias=atoi(value.c_str());
  }
  else if(key.compare( "target")==0){
    std::transform( value.begin(), value.end(), value.begin(),::tolower );
    card->spellTargetType=value;
  }
  else if(key.compare( "text")==0){
    card->setText(value);
  }else if (key.compare("abilities")==0){
    //Specific Abilities
    std::transform( value.begin(), value.end(), value.begin(),::tolower );
    while (value.size()){
      string attribute;
      size_t found2 = value.find(",");
      if (found2 != string::npos){
        attribute = value.substr(0,found2);
        value = value.substr(found2+1);
      }else{
        attribute = value;
        value = "";
      }
      for (int j = Constants::NB_BASIC_ABILITIES-1; j >=0 ; j--){
        size_t found = attribute.find(Constants::MTGBasicAbilities[j]);
        if (found != string::npos){
	        card->basicAbilities[j] = 1;
          break;
        }
      }
    }
  }else if(key.compare("id")==0){
    card->setMTGId(atoi(value.c_str()));
  }else if(key.compare("name")==0){
    card->setName(value);
  }else if(key.compare("rarity")==0){
    card->setRarity (value.c_str()[0]);
  }else if(key.compare("mana")==0){
    std::transform( value.begin(), value.end(), value.begin(),::tolower );
    card->setManaCost(value);
  }else if(key.compare("type")==0){
    switch(value.c_str()[0]){
    case 'C':
      card->setType( "Creature");
      break;
    case 'A':
      card->setType( "Artifact");
      card->setColor(Constants::MTG_COLOR_ARTIFACT);
      if (value.c_str()[8] == ' ' && value.c_str()[9] == 'C')
	      card->setSubtype("Creature");
      break;
    case 'E':
      card->setType( "Enchantment");
      break;
    case 'S':
      card->setType( "Sorcery");
      break;
    case 'B'://Basic Land
      card->setColor(Constants::MTG_COLOR_LAND);
      card->setType("Land");
      card->setType("Basic");
      break;
    case 'L':
      card->setColor(Constants::MTG_COLOR_LAND);
      card->setType( "Land");
      break;
    case 'I':
      card->setType( "Instant");
      break;
    default:
      card->setType( "Error");
      break;

    }
  }else if(key.compare("power")==0){
    card->setPower (atoi(value.c_str()));
  }else if(key.compare("subtype")==0){
    while (value.size()){
      unsigned int found = value.find(" ");
      if (found != string::npos){
        card->setSubtype(value.substr(0,found));
        value = value.substr(found+1);
      }else{
        card->setSubtype(value);
        value = "";
      }
    }
  }else if(key.compare("toughness")==0){
    card->setToughness(atoi(value.c_str()));
  }else{
  }


  return i;

}

void MTGAllCards::initCounters(){
  for (int i=0; i < Constants::MTG_NB_COLORS; i++){
    colorsCount[i] = NULL;
  }
}

void MTGAllCards::init(){
  tempCard = NULL;
  mCache = NULL;
  total_cards = 0;
  initCounters();
  srand(time(0));  // initialize random
}



int MTGAllCards::load(const char * config_file, const char * set_name,int autoload){
  conf_read_mode = 0;
  int set_id = MtgSets::SetsList->Add(set_name);

  std::ifstream setFile(config_file);

  if (setFile){
    while(readConfLine(setFile, set_id)){};
  }

  return total_cards;
}

MTGAllCards::MTGAllCards(){
  init();
}

MTGAllCards::~MTGAllCards(){
  //Why don't we call destroyAllCards from here ???
}

void MTGAllCards::destroyAllCards(){
  map<int,MTGCard *>::iterator it;

  for (it = collection.begin(); it!=collection.end(); it++) delete(it->second);

  collection.clear();
  ids.clear();

}

MTGAllCards::MTGAllCards(const char * config_file, const char * set_name){
  MTGAllCards(config_file, set_name, NULL);
}

MTGAllCards::MTGAllCards(TexturesCache * cache){
  init();
  mCache = cache;
}

MTGAllCards::MTGAllCards(const char * config_file, const char * set_name, TexturesCache * cache){
  init();
  mCache = cache;
  load(config_file,set_name, 0);
}



int MTGAllCards::randomCardId(){
  int id = (rand() % ids.size());
  return ids[id];
}



int MTGAllCards::countBySet(int setId){
  int result = 0;
  map<int,MTGCard *>::iterator it;

  for (it = collection.begin(); it!=collection.end(); it++){
    MTGCard * c = it->second;
    if( c->setId == setId){
      result++;
    }
  }
  return result;
}

//TODO more efficient way ?
int MTGAllCards::countByType(const char * _type){
  int result = 0;
  map<int,MTGCard *>::iterator it;
  for (it = collection.begin(); it!=collection.end(); it++){
    MTGCard * c = it->second;
    if(c->hasType(_type)){
      result++;
    }
  }
  return result;
}


int MTGAllCards::countByColor(int color){
  if (colorsCount[color] == 0){
    for (int i=0; i< Constants::MTG_NB_COLORS; i++){
      colorsCount[i] = 0;
    }
    map<int,MTGCard *>::iterator it;
    for (it = collection.begin(); it!=collection.end(); it++){
      MTGCard * c = it->second;
      int j = c->getColor();

      colorsCount[j]++;
    }
  }
  return colorsCount[color];
}

int MTGAllCards::totalCards(){
  return (total_cards);
}


int MTGAllCards::readConfLine(std::ifstream &file, int set_id){

  string s;
  int result = 1;
  if(!std::getline(file,s)) return 0;
  if (!s.size()) return -1;
  if (s[s.size()-1] == '\r') s.erase(s.size()-1); //Handle DOS files
    switch(conf_read_mode) {
    case 0:
      if (s[0] == '['){
        tempCard = NEW MTGCard(mCache,set_id);
        conf_read_mode = 1;
      }
      break;
    case 1:
      if (s[0] == '[' && s[1] == '/'){
	      conf_read_mode = 0;
        int newId = tempCard->getId();
        if (collection.find(newId) != collection.end()){
          char outBuf[4096];
          sprintf(outBuf,"warning, card id collision! : %i - %s\n", newId, tempCard->name.c_str());
          OutputDebugString (outBuf);
          delete tempCard;
        }else{
          ids.push_back(newId);
          collection[newId] = tempCard;
	        total_cards++;
        }
      }else{
	      processConfLine(s, tempCard);
      }
      break;
    default:
      break;
    }

 

  return result;

}


MTGCard * MTGAllCards::getCardById(int id){
	map<int, MTGCard *>::iterator it = collection.find(id);
	if ( it != collection.end()){
    return (it->second);
  }
  return 0;
}

MTGCard * MTGAllCards::_(int index){
  if (index >= total_cards) return NULL;
  return getCardById(ids[index]);
}

MTGCard * MTGAllCards::getCardByName(string name){
  if (!name.size()) return NULL;
  if (name[0] == '#') return NULL;
  std::transform(name.begin(), name.end(), name.begin(),::tolower );
  int setId = -1;
  size_t found = name.find(" (");
  if (found != string::npos){
    size_t end = name.find(")");
    string setName = name.substr(found+2,end-found-2);
    name = name.substr(0,found);
    setId = MtgSets::SetsList->find(setName);
  }
  map<int,MTGCard *>::iterator it;
  for (it = collection.begin(); it!=collection.end(); it++){
    MTGCard * c = it->second;
    if (setId!=-1 && setId != c->setId) continue;
    string cardName = c->name;
    std::transform(cardName.begin(), cardName.end(), cardName.begin(),::tolower );
    if (cardName.compare(name) == 0) return c;
    
  }
  return NULL;
}



MTGDeck::MTGDeck(TexturesCache * cache, MTGAllCards * _allcards){
  mCache = cache;
  total_cards = 0;
  database = _allcards;
}

MTGDeck::MTGDeck(const char * config_file, TexturesCache * cache, MTGAllCards * _allcards, int meta_only){
  mCache = cache;
  total_cards = 0;
  database = _allcards;
  filename = config_file;
  size_t slash = filename.find_last_of("/");
  size_t dot = filename.find(".");
  meta_name = filename.substr(slash+1,dot-slash-1);
  std::ifstream file(config_file);
  std::string s;

  if(file){
    while(std::getline(file,s)){
      if (!s.size()) continue;
      if (s[s.size()-1] == '\r') s.erase(s.size()-1); //Handle DOS files
      if (s[0] == '#'){
        size_t found = s.find("NAME:");
        if ( found != string::npos){
          meta_name = s.substr(found+5);
          continue;
        }
        found = s.find("DESC:");
        if ( found != string::npos){
          if (meta_desc.size()) meta_desc.append("\n");
          meta_desc.append(s.substr(found+5));
          continue;
        }
        continue;
      }
      if (meta_only) break;
      int cardnb = atoi(s.c_str());
      if (cardnb){
        add(cardnb);
      }else{
        int nb = 1;
        size_t found = s.find(" *");
        if (found != string::npos){
          nb = atoi(s.substr(found+2).c_str());
          s=s.substr(0,found);
          OutputDebugString(s.c_str());
        }
        MTGCard * card = database->getCardByName(s);
        if (card){
          for (int i = 0; i < nb; i++){
            add(card);
          }
        }
      }
    }
    file.close();
  }else{
    //TODO Error management
  }


}

int MTGDeck::totalCards(){
  return total_cards;
}

MTGCard * MTGDeck::getCardById(int mtgId){
  return database->getCardById(mtgId);
}

int MTGDeck::addRandomCards(int howmany, int setId, int rarity, const char * _subtype){
  int collectionTotal = database->totalCards();
  if (!collectionTotal) return 0;
  if (setId == -1 && rarity == -1 && !_subtype){
    for (int i = 0; i < howmany; i++){
      add(database->randomCardId());
    }
    return 1;
  }
  char subtype[4096];
  if (_subtype)
    sprintf(subtype, _subtype);


  vector<int> subcollection;
  int subtotal = 0;
  for (int i = 0; i < collectionTotal; i++){
    MTGCard * card = database->_(i);
    if ((setId == -1 || card->setId == setId) &&
	(rarity == -1 || card->getRarity()==rarity) &&
	(!_subtype || card->hasSubtype(subtype))
	){
      subcollection.push_back(card->getId());
      subtotal++;
    }
  }
  if (subtotal == 0) return 0;
  for (int i = 0; i < howmany; i++){
    int id = (rand() % subtotal);
    add(subcollection[id]);
  }
  return 1;
}

int MTGDeck::add(MTGDeck * deck){
  map<int,int>::iterator it;
  for (it = deck->cards.begin(); it!=deck->cards.end(); it++){
    for (int i = 0; i < it->second; i++){
      add(it->first);
    }
  }
  return deck->totalCards();
}

int MTGDeck::add(int cardid){
  if (!database->getCardById(cardid)) return 0;
  if(cards.find(cardid) == cards.end()){
    cards[cardid] = 1;
  }else{
    cards[cardid]++;
  }
  ++total_cards;
  //initCounters();
  return total_cards;
}

int MTGDeck::add(MTGCard * card){
  if (!card) return 0;
  return (add(card->getId()));
}


int MTGDeck::removeAll(){
  total_cards = 0;
  cards.clear();
  //initCounters();
  return 1;
}

int MTGDeck::remove(int cardid){
  if(cards.find(cardid) == cards.end() || cards[cardid] == 0) return 0;
  cards[cardid]--;
  total_cards--;
  //initCounters();
  return 1;
}

int MTGDeck::remove(MTGCard * card){
  if (!card) return 0;
  return (remove(card->getId()));
}

int MTGDeck::save(){
  std::ofstream file(filename.c_str());
  char writer[10];
  if (file){
#if defined (WIN32) || defined (LINUX)
    OutputDebugString("saving");
#endif
    map<int,int>::iterator it;
    for (it = cards.begin(); it!=cards.end(); it++){
      sprintf(writer,"%i\n", it->first);
      for (int j = 0; j<it->second; j++){
        file<<writer;
      }
    }
    file.close();
  }

  return 1;
}



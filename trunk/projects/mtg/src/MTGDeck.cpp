#include "../include/debug.h"
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


int MTGAllCards::processConfLine(char *buffer, MTGCard *card){
  string s = buffer;
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
    for (int j = 0; j < NB_BASIC_ABILITIES; j++){
      unsigned int found = value.find(MTGBasicAbilities[j]);
      if (found != string::npos){
	card->basicAbilities[j] = 1;
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
      card->setColor(MTG_COLOR_ARTIFACT);
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
      card->setColor(MTG_COLOR_LAND);
      card->setType("Land");
      break;
    case 'L':
      card->setColor(MTG_COLOR_LAND);
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
    card->setSubtype(value);
  }else if(key.compare("toughness")==0){
    card->setToughness(atoi(value.c_str()));
  }else{
  }


  return i;

}

void MTGAllCards::initCounters(){
  for (int i=0; i< MTG_NB_COLORS; i++){
    colorsCount[i] = NULL;
  }
}

void MTGAllCards::init(){
  mCache = NULL;
  total_cards = 0;
  initCounters();
  srand(time(0));  // initialize random
}



int MTGAllCards::load(const char * config_file, const char * set_name,int autoload){
  conf_read_mode = 0;
  int file_size = filesize(config_file);
  conf_buffer = (char *) malloc(file_size);
  read_cursor = 0;
  //  conf_fd = sceIoOpen(config_file, PSP_O_RDONLY, 0777);
  read_file(config_file, conf_buffer, file_size );
  int set_id = MtgSets::SetsList->Add(set_name);
  if (autoload){
    while(readConfLine(set_id)){};
  }
  return total_cards;
}

MTGAllCards::MTGAllCards(){
  init();
}

MTGAllCards::~MTGAllCards(){
}

void MTGAllCards::destroyAllCards(){
  for (int i= 0; i < total_cards; i++){
    delete collection[i];
  };

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


MTGCard * MTGAllCards::_(int i){
  if (i < total_cards) return collection[i];
  return NULL;
}


int MTGAllCards::randomCardId(){
  int id = (rand() % total_cards);
  return collection[id]->getMTGId();
}



int MTGAllCards::countBySet(int setId){
  int result = 0;
  for (int i=0; i< total_cards; i++){
    if(collection[i]->setId == setId){
      result++;
    }
  }
  return result;
}

//TODO more efficient way ?
int MTGAllCards::countByType(const char * _type){
  int result = 0;
  for (int i=0; i< total_cards; i++){
    if(collection[i]->hasType(_type)){
      result++;
    }
  }
  return result;
}


int MTGAllCards::countByColor(int color){
  if (colorsCount[color] == 0){
    for (int i=0; i< MTG_NB_COLORS; i++){
      colorsCount[i] = 0;
    }
    for (int i=0; i< total_cards; i++){
      int j = collection[i]->getColor();

      colorsCount[j]++;
    }
  }
  return colorsCount[color];
}

int MTGAllCards::totalCards(){
  return (total_cards);
}

int MTGAllCards::readConfLine(int set_id){
  char buffer[BUFSIZE];
  read_cursor = readline(conf_buffer, buffer,  read_cursor);
  if (read_cursor){
    switch(conf_read_mode) {
    case 0:
      conf_read_mode = 1;
      collection[total_cards] =  NEW MTGCard(mCache,set_id);
      break;
    case 1:
      if (buffer[0] == '[' && buffer[1] == '/'){
	conf_read_mode = 0;
	total_cards++;
      }else{
	processConfLine(buffer, collection[total_cards]);
      }
      break;
    default:
      break;
    }

  }else{
    free (conf_buffer);
  }

  return read_cursor;


}


MTGCard * MTGAllCards::getCardById(int id){
  int i;
  for (i=0; i<total_cards; i++){
    int cardId = collection[i]->getMTGId();
    if (cardId == id){
      return collection[i];
    }
  }
  return 0;
}



MTGDeck::MTGDeck(const char * config_file, TexturesCache * cache, MTGAllCards * _allcards){
  mCache = cache;
  total_cards = 0;
  allcards = _allcards;
  filename = config_file;
  std::ifstream file(config_file);
  std::string s;

  if(file){
    while(std::getline(file,s)){
      int cardnb = atoi(s.c_str());
      if (cardnb) add(cardnb);
    }
    file.close();
  }else{
    //TODO Error management
  }


}



int MTGDeck::addRandomCards(int howmany, int setId, int rarity, const char * _subtype){
  int collectionTotal = allcards->totalCards();
  if (!collectionTotal) return 0;
  if (setId == -1 && rarity == -1 && !_subtype){
    for (int i = 0; i < howmany; i++){
      int id = (rand() % collectionTotal);
      add(allcards->_(id));
    }
    return 1;
  }
  char subtype[4096];
  if (_subtype)
    sprintf(subtype, _subtype);


  int subcollection[TOTAL_NUMBER_OF_CARDS];
  int subtotal = 0;
  for (int i = 0; i < collectionTotal; i++){
    MTGCard * card = allcards->_(i);
    if ((setId == -1 || card->setId == setId) &&
	(rarity == -1 || card->getRarity()==rarity) &&
	(!_subtype || card->hasSubtype(subtype))
	){
      subcollection[subtotal] = i;
      subtotal++;
    }
  }
  if (subtotal == 0) return 0;
  for (int i = 0; i < howmany; i++){
    int id = (rand() % subtotal);
    add(allcards->_(subcollection[id]));
  }
  return 1;
}

int MTGDeck::add(int cardid){
  MTGCard * card = allcards->getCardById(cardid);
  add(card);
  return total_cards;
}

int MTGDeck::add(MTGCard * card){
  if (!card) return 0;
  collection[total_cards] = card;
  ++total_cards;
  initCounters();
  return total_cards;
}


int MTGDeck::removeAll(){
  total_cards = 0;
  initCounters();
  return 1;
}

int MTGDeck::remove(int cardid){
  MTGCard * card = getCardById(cardid);
  return remove(card);
}

int MTGDeck::remove(MTGCard * card){
  for (int i = 0; i<total_cards; i++){
    if (collection[i] == card){
      collection[i] = collection[total_cards - 1];
      total_cards--;
      initCounters();
      return 1;
    }
  }
  return 0;
}

int MTGDeck::save(){
  std::ofstream file(filename.c_str());
  char writer[10];
  if (file){
#if defined (WIN32) || defined (LINUX)
    OutputDebugString("saving");
#endif
    for (int i = 0; i<total_cards; i++){
      sprintf(writer,"%i\n", collection[i]->getMTGId());
      file<<writer;
    }
    file.close();
  }

  return 1;
}

#include "../include/config.h"
#include "../include/MTGRules.h"
#include "../include/Translate.h"
#include "../include/Subtypes.h"
#include "../include/GameOptions.h"

MTGPutInPlayRule::MTGPutInPlayRule(int _id):MTGAbility(_id, NULL){
  aType=MTGAbility::PUT_INTO_PLAY;
}

int MTGPutInPlayRule::isReactingToClick(MTGCardInstance * card, ManaCost * mana){
int cardsinhand = game->players[0]->game->hand->nb_cards;
  Player * player = game->currentlyActing();
  Player * currentPlayer = game->currentPlayer;
  if (!player->game->hand->hasCard(card)) return 0;
  if ((game->turn < 1) && (cardsinhand != 0) && (card->hasType("leyline"))
	    && game->currentGamePhase == Constants::MTG_PHASE_FIRSTMAIN
	    && game->players[0]->game->graveyard->nb_cards == 0
		&& game->players[0]->game->exile->nb_cards == 0){
	    Player * p = game->currentPlayer;
		card->owner->game->putInZone(card,p->game->hand,p->game->battlefield);
			return 1;}
  if (card->hasType("land")){
    if (player == currentPlayer && currentPlayer->canPutLandsIntoPlay && (game->currentGamePhase == Constants::MTG_PHASE_FIRSTMAIN || game->currentGamePhase == Constants::MTG_PHASE_SECONDMAIN)){
      return 1;
    }
  }else if ((card->hasType("instant")) || card->has(Constants::FLASH) || (player == currentPlayer && !game->isInterrupting && (game->currentGamePhase == Constants::MTG_PHASE_FIRSTMAIN || game->currentGamePhase == Constants::MTG_PHASE_SECONDMAIN))){
    ManaCost * playerMana = player->getManaPool();
    ManaCost * cost = card->getManaCost();
#ifdef WIN32
  cost->Dump();
#endif
    if (playerMana->canAfford(cost)){
      return 1;
    }
  }
  return 0;
}
int MTGPutInPlayRule::reactToClick(MTGCardInstance * card){
  if (!isReactingToClick(card)) return 0;
  Player * player = game->currentlyActing();
  ManaCost * cost = card->getManaCost();
  if (cost->isExtraPaymentSet()){
    if (!game->targetListIsSet(card)){
      return 0;
	}
  }else{
    cost->setExtraCostsAction(this, card);
    game->waitForExtraPayment = cost->extraCosts;
    return 0;
  }
  ManaCost * previousManaPool = NEW ManaCost(player->getManaPool());
  int payResult = player->getManaPool()->pay(card->getManaCost());
  card->getManaCost()->doPayExtra();
  ManaCost * spellCost = previousManaPool->Diff(player->getManaPool());
  delete previousManaPool;
  if (card->hasType("land")){
    MTGCardInstance * copy = player->game->putInZone(card,  player->game->hand, player->game->temp);
    Spell * spell = NEW Spell(copy);
    spell->resolve();
    delete spellCost;
    delete spell;
    player->canPutLandsIntoPlay--;
  }else{
    Spell * spell = NULL;
    MTGCardInstance * copy = player->game->putInZone(card,  player->game->hand, player->game->stack);
    if (game->targetChooser){    
      spell = game->mLayers->stackLayer()->addSpell(copy,game->targetChooser, spellCost,payResult);
      game->targetChooser = NULL;
    }else{
      spell = game->mLayers->stackLayer()->addSpell(copy,NULL, spellCost, payResult);
    }
     copy->X = spell->computeX(copy); 
  }
  return 1;
}

//The Put into play rule is never destroyed
int MTGPutInPlayRule::testDestroy(){
  return 0;
}

ostream& MTGPutInPlayRule::toString(ostream& out) const
{
  out << "MTGPutInPlayRule ::: (";
  return MTGAbility::toString(out) << ")";
}

 MTGPutInPlayRule * MTGPutInPlayRule::clone() const{
    MTGPutInPlayRule * a =  NEW MTGPutInPlayRule(*this);
    a->isClone = 1;
    return a;
  }
//cast from anywhere possible with this??


bool MTGAttackRule::select(Target* t)
{
  if (CardView* c = dynamic_cast<CardView*>(t)) {
    MTGCardInstance * card = c->getCard();
    if (card->canAttack()) return true;
  }
  return false;
}
bool MTGAttackRule::greyout(Target* t)
{
  return true;
}

MTGAttackRule::MTGAttackRule(int _id):MTGAbility(_id,NULL){
  aType=MTGAbility::MTG_ATTACK_RULE;
}

int MTGAttackRule::isReactingToClick(MTGCardInstance * card, ManaCost * mana){
  if (currentPhase == Constants::MTG_PHASE_COMBATATTACKERS && card->controller() == game->currentPlayer){
    if (card->isAttacker()) return 1;
    if (card->canAttack()) return 1;
  }
  return 0;
}

int MTGAttackRule::receiveEvent(WEvent *e){
  if (WEventPhaseChange* event = dynamic_cast<WEventPhaseChange*>(e)) {
    if (Constants::MTG_PHASE_COMBATATTACKERS == event->from->id) {
      Player * p = game->currentPlayer;
      MTGGameZone * z = p->game->inPlay;
      for (int i= 0; i < z->nb_cards; i++){
        MTGCardInstance * card = z->cards[i];
        if (!card->isAttacker() && card->has(Constants::MUSTATTACK)) reactToClick(card);
      }
      return 1;
    }
  }
  return 0;
}

int MTGAttackRule::reactToClick(MTGCardInstance * card){
  if (!isReactingToClick(card)) return 0;

  //Graphically select the next card that can attack
  if(!card->isAttacker()){
    CardSelector * cs = game->mLayers->cs;
    cs->PushLimitor();
    cs->Limit(this,CardSelector::playZone);
    cs->CheckUserInput(JGE_BTN_RIGHT);
    cs->Limit(NULL,CardSelector::playZone);
    cs->PopLimitor();
  }
  card->toggleAttacker();
  return 1;
}

//The Attack rule is never destroyed
int MTGAttackRule::testDestroy(){
  return 0;
}

ostream& MTGAttackRule::toString(ostream& out) const
{
  out << "MTGAttackRule ::: (";
  return MTGAbility::toString(out) << ")";
}

 MTGAttackRule * MTGAttackRule::clone() const{
    MTGAttackRule * a =  NEW MTGAttackRule(*this);
    a->isClone = 1;
    return a;
  }


OtherAbilitiesEventReceiver::OtherAbilitiesEventReceiver(int _id):MTGAbility(_id,NULL){
}


int OtherAbilitiesEventReceiver::receiveEvent(WEvent *e){
  if (WEventZoneChange* event = dynamic_cast<WEventZoneChange*>(e)) {
    if (event->to && (event->to != event->from)){
      GameObserver * g = GameObserver::GetInstance();
      for (int i = 0; i < 2; ++i){
        if (event->to == g->players[i]->game->inPlay) return 0;
      }
      AbilityFactory af;
      af.magicText(g->mLayers->actionLayer()->getMaxId(), NULL, event->card, 1, 0,event->to);
      return 1;
	}
  }
  return 0;
}

int OtherAbilitiesEventReceiver::testDestroy(){
  return 0;
}

 OtherAbilitiesEventReceiver * OtherAbilitiesEventReceiver::clone() const{
    OtherAbilitiesEventReceiver * a =  NEW OtherAbilitiesEventReceiver(*this);
    a->isClone = 1;
    return a;
  }


MTGBlockRule::MTGBlockRule(int _id):MTGAbility(_id,NULL){
  aType=MTGAbility::MTG_BLOCK_RULE;
}

int MTGBlockRule::isReactingToClick(MTGCardInstance * card, ManaCost * mana){
  if (currentPhase == Constants::MTG_PHASE_COMBATBLOCKERS && !game->isInterrupting && card->controller() == game->currentlyActing()){
    if (card->canBlock()) return 1;
  }
  return 0;
}

int MTGBlockRule::reactToClick(MTGCardInstance * card){
  if (!isReactingToClick(card)) return 0;
  MTGCardInstance * currentOpponent = card->isDefenser();

  bool result = false;
  int candefend = 0;
  while (!result){
    currentOpponent = game->currentPlayer->game->inPlay->getNextAttacker(currentOpponent);
#if defined (WIN32) || defined (LINUX)
    char buf[4096];
    sprintf(buf,"Defenser Toggle %s \n", card->getName().c_str());
    OutputDebugString(buf);
#endif
    candefend = card->toggleDefenser(currentOpponent);
    result = (candefend || currentOpponent == NULL);
  }
  return 1;
}



//The Block rule is never destroyed
int MTGBlockRule::testDestroy(){
  return 0;
}

ostream& MTGBlockRule::toString(ostream& out) const
{
  out << "MTGBlockRule ::: (";
  return MTGAbility::toString(out) << ")";
}

 MTGBlockRule * MTGBlockRule::clone() const{
    MTGBlockRule * a =  NEW MTGBlockRule(*this);
    a->isClone = 1;
    return a;
  }
//
// Attacker chooses blockers order
//

//
// * Momir
//

int MTGMomirRule::initialized = 0;
vector<int> MTGMomirRule::pool[20];

MTGMomirRule::MTGMomirRule(int _id, MTGAllCards * _collection):MTGAbility(_id, NULL){
  collection = _collection;
  if (!initialized){
    for (size_t i = 0; i < collection->ids.size(); i++){
      MTGCard * card = collection->collection[collection->ids[i]];
      if (card->data->isCreature()){
         int convertedCost = card->data->getManaCost()->getConvertedCost();
         if (convertedCost>20) continue;
         pool[convertedCost].push_back(card->getMTGId());
      }
    }
    initialized =1;
  }
  alreadyplayed = 0;
  aType=MTGAbility::MOMIR;
  textAlpha = 0;
}

int MTGMomirRule::isReactingToClick(MTGCardInstance * card, ManaCost * mana){
  if (alreadyplayed) return 0;
  Player * player = game->currentlyActing();
  Player * currentPlayer = game->currentPlayer;
  if (!player->game->hand->hasCard(card)) return 0;
  if (player == currentPlayer && !game->isInterrupting && (game->currentGamePhase == Constants::MTG_PHASE_FIRSTMAIN || game->currentGamePhase == Constants::MTG_PHASE_SECONDMAIN)){
	return 1;
  }
  return 0;
}

int MTGMomirRule::reactToClick(MTGCardInstance * card_to_discard){
  Player * player = game->currentlyActing();
  ManaCost * cost = player->getManaPool();
  int converted = cost->getConvertedCost();
  int id = genRandomCreatureId(converted);
  return reactToClick(card_to_discard, id);
}

int MTGMomirRule::reactToClick(MTGCardInstance * card_to_discard, int cardId){
  if (!isReactingToClick(card_to_discard)) return 0;
   Player * player = game->currentlyActing();
  ManaCost * cost = player->getManaPool();
  player->getManaPool()->pay(cost);
   MTGCardInstance * card = genCreature(cardId);
  player->game->putInZone(card_to_discard,  player->game->hand, player->game->graveyard);

  player->game->stack->addCard(card);
  Spell * spell = NEW Spell(card);
  spell->resolve();
  spell->source->isToken = 1;
  delete spell;
	alreadyplayed = 1;
  textAlpha = 255;
  text = card->name;
  return 1;
}

MTGCardInstance * MTGMomirRule::genCreature( int id){
  if (!id) return NULL;
  Player * p = game->currentlyActing();
  MTGCard * card = collection->getCardById(id);
  return NEW MTGCardInstance(card,p->game);
}

int MTGMomirRule::genRandomCreatureId(int convertedCost){
   if (convertedCost >= 20) convertedCost = 19;
   int total_cards = 0;
   int i = convertedCost;
   while (!total_cards && i >=0){
#ifdef WIN32
     char buf[4096];
     sprintf(buf,"Converted Cost in momir: %i\n", i);
     OutputDebugString(buf);
#endif
    total_cards = pool[i].size();
    convertedCost = i;
    i--;
   }
   if (!total_cards) return 0;
   int start = (WRand() % total_cards);
   return pool[convertedCost][start];
}

//The Momir rule is never destroyed
int MTGMomirRule::testDestroy(){
  return 0;
}

void MTGMomirRule::Update(float dt){
  if (newPhase != currentPhase && newPhase == Constants::MTG_PHASE_UNTAP){
    alreadyplayed = 0;
  }
  if (textAlpha){
    textAlpha -= (200*dt);
    if (textAlpha <0) textAlpha = 0;
  }
  MTGAbility::Update(dt);
}

void MTGMomirRule::Render(){
  if (!textAlpha) return;
  WFont * mFont = resources.GetWFont(Constants::MENU_FONT);
  mFont->SetScale(2 - (float)textAlpha/130);
  mFont->SetColor(ARGB(textAlpha,255,255,255));
  mFont->DrawString(text.c_str(),SCREEN_WIDTH/2,SCREEN_HEIGHT/2,JGETEXT_CENTER);
}

ostream& MTGMomirRule::toString(ostream& out) const
{
  out << "MTGMomirRule ::: pool : " << pool << " ; initialized : " << initialized << " ; textAlpha : " << textAlpha << " ; text " << text << " ; alreadyplayed : " << alreadyplayed << " ; collection : " << collection << "(";
  return MTGAbility::toString(out) << ")";
}


 MTGMomirRule * MTGMomirRule::clone() const{
    MTGMomirRule * a =  NEW MTGMomirRule(*this);
    a->isClone = 1;
    return a;
  }

//HUDDisplay
int HUDDisplay::testDestroy(){
  return 0;
}

void HUDDisplay::Update(float dt){
  timestamp += dt;
  popdelay +=dt;
  if (events.size()){
    list<HUDString *>::iterator it = events.begin();
    HUDString * hs = *it;
    if (popdelay > 1 && timestamp - hs->timestamp > 2){
      events.pop_front();
      delete hs;
      if (events.size()) popdelay = 0;
    }
  }else{
    maxWidth = 0;
  }
}

int HUDDisplay::addEvent(string s){
  events.push_back(NEW HUDString(s, timestamp));
  float width = f->GetStringWidth(s.c_str());
  if (width > maxWidth) maxWidth = width;
  return 1;
}

int HUDDisplay::receiveEvent(WEvent * event){

  WEventZoneChange * ezc = dynamic_cast<WEventZoneChange*>(event);
  if (ezc) {
    for (int i = 0; i < 2 ; i++){
      Player * p = game->players[i];
      if (ezc->to == p->game->graveyard ){
        char buffer[512];
        sprintf(buffer,_("%s goes to graveyard").c_str(), _(ezc->card->getName()).c_str());
        string s = buffer;
        return addEvent(s);
      }
    }
  }

  WEventDamage * ed = dynamic_cast<WEventDamage*>(event);
  if (ed) {
    char buffer[512];
    sprintf(buffer, "%s: %i -> %s", _(ed->damage->source->name).c_str(), ed->damage->damage, _(ed->damage->target->getDisplayName()).c_str());
    string s = buffer;
    return addEvent(s);
  }

  return 0;
}
void HUDDisplay::Render(){
  if (!options[Options::OSD].number) return;
  if (!events.size()) return;

  f->SetColor(ARGB(255,255,255,255));

  list<HUDString *>::reverse_iterator it;

  float x0 = SCREEN_WIDTH-10-maxWidth-10;
  float y0 = 20;
  float size = events.size() * 16;
  JRenderer * r = JRenderer::GetInstance();
  r->FillRoundRect(x0,y0,maxWidth + 10,size,5,ARGB(50,0,0,0));

  int i = 0;
  for (it = events.rbegin(); it !=events.rend(); ++it){
    HUDString * hs = *it;
    f->DrawString(hs->value.c_str(),x0 + 5, y0 + 16 * i);
    i++;
  }
}
HUDDisplay::HUDDisplay(int _id):MTGAbility(_id, NULL){
  timestamp = 0;
  popdelay = 2;
  f = resources.GetWFont(Constants::MAIN_FONT);
  maxWidth = 0;
}

HUDDisplay::~HUDDisplay(){
  list<HUDString *>::iterator it;
  for (it = events.begin(); it !=events.end(); ++it){
    HUDString * hs = *it;
    delete hs;
  }
  events.clear();
}

  HUDDisplay * HUDDisplay::clone() const{
    HUDDisplay * a =  NEW HUDDisplay(*this);
    a->isClone = 1;
    return a;
  }


  /* Persist */
  MTGPersistRule::MTGPersistRule(int _id):MTGAbility(_id,NULL){};

  int MTGPersistRule::receiveEvent(WEvent * event){
    if (event->type == WEvent::CHANGE_ZONE){
      WEventZoneChange * e = (WEventZoneChange *) event;
      MTGCardInstance * card = e->card->previous;
      if (card && card->basicAbilities[Constants::PERSIST] && !card->counters->hasCounter(-1,-1)){
        int ok = 0;
        for (int i = 0; i < 2 ; i++){
          Player * p = game->players[i];
          if (e->from == p->game->inPlay) ok = 1;
        }
        if (!ok) return 0;
        for (int i = 0; i < 2 ; i++){
          Player * p = game->players[i];
          if (e->to == p->game->graveyard){
	          MTGCardInstance * copy = p->game->putInZone(e->card,  p->game->graveyard, e->card->owner->game->stack);
            Spell * spell = NEW Spell(copy);
	          spell->resolve();
            spell->source->counters->addCounter(-1,-1);
            delete spell;
            return 1;
          }
        }
      }
    }
    return 0;
  }

  ostream& MTGPersistRule::toString(ostream& out) const
  {
    out << "MTGPersistRule ::: (";
    return MTGAbility::toString(out) << ")";
  }
  int MTGPersistRule::testDestroy(){return 0;}
  MTGPersistRule * MTGPersistRule::clone() const{
    MTGPersistRule * a =  NEW MTGPersistRule(*this);
    a->isClone = 1;
    return a;
  }


  MTGTokensCleanup::MTGTokensCleanup(int _id):MTGAbility(_id, NULL){}

  int MTGTokensCleanup::receiveEvent(WEvent * e){
    if (WEventZoneChange* event = dynamic_cast<WEventZoneChange*>(e)){
      if (!event->card->isToken) return 0;
      if (event->to == game->players[0]->game->inPlay || event->to == game->players[1]->game->inPlay) return 0;
      if (event->to == game->players[0]->game->garbage || event->to == game->players[1]->game->garbage) return 0;
      list.push_back(event->card);
      return 1;
    }
    return 0;
  }

  int MTGTokensCleanup::testDestroy(){return 0;}

  void MTGTokensCleanup::Update(float dt){
    MTGAbility::Update(dt);
    for(size_t i= 0; i < list.size(); ++i){
      MTGCardInstance * c = list[i];
      c->controller()->game->putInZone(c,c->currentZone, c->controller()->game->garbage);
    }
    list.clear();
  }

  MTGTokensCleanup *  MTGTokensCleanup::clone() const{
    MTGTokensCleanup * a =  NEW MTGTokensCleanup(*this);
    a->isClone = 1;
    return a;
  }

  /* Legend Rule */
  MTGLegendRule::MTGLegendRule(int _id):ListMaintainerAbility(_id){};

  int MTGLegendRule::canBeInList(MTGCardInstance * card){
    if (card->hasType(Subtypes::TYPE_LEGENDARY) && game->isInPlay(card)){
      return 1;
    }
    return 0;
  }

  int MTGLegendRule::added(MTGCardInstance * card){
    map<MTGCardInstance *,bool>::iterator it;
    int destroy = 0;
    for ( it=cards.begin() ; it != cards.end(); it++ ){
      MTGCardInstance * comparison = (*it).first;
      if (comparison != card && !(comparison->getName().compare(card->getName()))){
	      comparison->controller()->game->putInGraveyard(comparison);
	      destroy = 1;
      }
    }
    if (destroy){
      card->owner->game->putInGraveyard(card);
    }
    return 1;
  }

  int MTGLegendRule::removed(MTGCardInstance * card){return 0;}

  int MTGLegendRule::testDestroy(){return 0;}

  ostream& MTGLegendRule::toString(ostream& out) const
  {
    return out << "MTGLegendRule :::";
  }
  MTGLegendRule * MTGLegendRule::clone() const{
    MTGLegendRule * a =  NEW MTGLegendRule(*this);
    a->isClone = 1;
    return a;
  }

  /* Lifelink */
  MTGLifelinkRule::MTGLifelinkRule(int _id):MTGAbility(_id,NULL){};

  int  MTGLifelinkRule::receiveEvent(WEvent * event){
    if (event->type == WEvent::DAMAGE){
      WEventDamage * e = (WEventDamage *) event;
      Damage * d = e->damage;
      MTGCardInstance * card = d->source;
      if (d->damage>0 && card && card->basicAbilities[Constants::LIFELINK]){
        card->controller()->life+= d->damage;
        return 1;
      }
    }
    return 0;
  }

  int  MTGLifelinkRule::testDestroy(){return 0;}

 ostream&  MTGLifelinkRule::toString(ostream& out) const
  {
    out << "MTGLifelinkRule ::: (";
    return MTGAbility::toString(out) << ")";
  }
  MTGLifelinkRule *  MTGLifelinkRule::clone() const{
        MTGLifelinkRule * a =  NEW MTGLifelinkRule(*this);
    a->isClone = 1;
    return a;
  }


   /* Deathtouch */
  MTGDeathtouchRule::MTGDeathtouchRule(int _id):MTGAbility(_id,NULL){};

  int  MTGDeathtouchRule::receiveEvent(WEvent * event){
    if (event->type == WEvent::DAMAGE){
      WEventDamage * e = (WEventDamage *) event;

      Damage * d = e->damage;
      if (d->damage <= 0) return 0;

      MTGCardInstance * card = d->source;
      if (!card) return 0;

      if (d->target->type_as_damageable != DAMAGEABLE_MTGCARDINSTANCE) return 0;
      MTGCardInstance * _target =  (MTGCardInstance *) (d->target);
      
      if (card->basicAbilities[Constants::DEATHTOUCH]){
        _target->destroy();
        return 1;
      }
    }
    return 0;
  }

  int  MTGDeathtouchRule::testDestroy(){return 0;}

 
  MTGDeathtouchRule *  MTGDeathtouchRule::clone() const{
    MTGDeathtouchRule * a =  NEW MTGDeathtouchRule(*this);
    a->isClone = 1;
    return a;
  }


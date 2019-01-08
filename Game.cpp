#include "Game.h"



namespace SwissalpS { namespace QtNibblers {



Game::Game(QObject *pParent) :
	QObject(pParent),
	bLevelStarted(false),
	bPaused(true),
	ubCountApplesLeft(13u),
	ubCountDead(0u),
	ubCountLevels(0u),
	ubSpeedIndex(0u),
	uiApplesToGo(0u),
	fFactorApple(1.0f),
	fFactorBanana(1.0f),
	fFactorCherry(1.0f),
	fFractionApple(0.0f),
	fFractionBanana(0.0f),
	fFractionCherry(0.0f),
	pAS(AppSettings::pAppSettings()) {

	this->apBonus.clear();
	this->apWorms.clear();

} // construct


Game::~Game() {

	this->apBonus.clear();
	this->apWorms.clear();

	if (this->pTimer) {
		this->pTimer->stop();
		this->pTimer = nullptr;
		delete this->pTimer;
	}

} // dealloc


void Game::destroyBonus(Bonus *pBonus) {

	this->disconnect(pBonus);
	this->apBonus.removeOne(pBonus);
	pBonus->deleteLater();

} // destroyBonus


void Game::init() {

	this->pTimer = new QTimer(this);
	this->pTimer->setSingleShot(false);
	this->onSpeedChanged(this->pAS->get(AppSettings::sSettingGameSpeed).toInt());

	connect(this->pTimer, SIGNAL(timeout()),
			this, SLOT(onTick()));

} // init


void Game::onBonusPlaced(const QVector<SurfaceCell *> apCells) {

	Bonus *pBonus = new Bonus(apCells, this);

	connect(this, SIGNAL(move()),
			pBonus, SLOT(onTick()));

	connect(pBonus, SIGNAL(debugMessage(QString)),
			this, SLOT(onDebugMessage(QString)));

	connect(pBonus, SIGNAL(timedOut(Bonus*)),
			this, SLOT(onBonusTimedOut(Bonus*)));

	this->apBonus.prepend(pBonus);

	// wich kind? how many ticks until expired?
	quint16 uiTicks = 0u;
	switch (pBonus->getStateBase()) {

		case 100u: // apple

			uiTicks = SssS_Nibblers_Bonus_Apple_Lifetime;

		break;

		case 110u: // cherry

			uiTicks = SssS_Nibblers_Bonus_Cherry_Lifetime;

		break;

		case 120u: // banana

			uiTicks = SssS_Nibblers_Bonus_Banana_Lifetime;

		break;

		case 130u: // heart

			uiTicks = SssS_Nibblers_Bonus_Heart_Lifetime;

		break;

		case 140u: // diamond

			uiTicks = SssS_Nibblers_Bonus_Diamond_Lifetime;

		break;

		default:
			this->onDebugMessage("unknown bonus encountered in Game::onBonusPlaced "
								 + QString::number(pBonus->getStateBase()));
		break;

	} // switch pBonus->getStateBase()

	pBonus->start(uiTicks);

} // onBonusPlaced


void Game::onBonusTimedOut(Bonus *pBonus) {

	// TODO:
	// what kind? do we need to place it again?

	qint16 iPenalty = -1 * SssS_Nibblers_Penalty_Non_Pickup
					  * this->ubCountLevels	* this->ubSpeedIndex;

	for (int i = 0; i < this->apWorms.length(); ++i) {

		this->apWorms.at(i)->onAddScore(iPenalty);

	} // loop worms

	// destroy Bonus
	this->destroyBonus(pBonus);

} // onBonusTimedOut


void Game::onNoSpaceFoundForBonus(const quint8 ubBonus) {

	// TODO:

} // onNoSpaceFoundForBonus


void Game::onPlayerCountChanged(const quint8 ubCountHumans,
								const quint8 ubCountAIs) {

	// TODO:

} // onPlayerCountChanged


void Game::onReset() {

	this->ubCountDead = 0u;
	this->ubCountLevels = 0u;
	this->bLevelStarted = false;

	quint8 ubLives = this->pAS->get(AppSettings::sSettingGameStartLives).toUInt();

	Worm *pWorm;
	for (int i = 0; i < this->apWorms.length(); ++i) {

		pWorm = this->apWorms.at(i);

		pWorm->onSetLives(ubLives);
		pWorm->onResetScore();
		pWorm->startSpawning();

	} // loop

} // onReset


void Game::onPauseResumeToggled() {

	if (this->isGameOver()) return;

	this->bPaused = !this->bPaused;

	if (this->bLevelStarted) {

		if (this->bPaused) {

			// do whatever to pause game

			this->pTimer->stop();

		} else {

			// do whatever to resume game

			this->pTimer->start();

		} // if pause or resume game

		return;

	} // if level already started

	// not yet started
	this->bLevelStarted = true;
	this->bPaused = true;

	for (int iCount = 0; iCount < this->apWorms.length(); ++iCount) {

		Q_EMIT this->spawnWorm(this->apWorms.at(iCount));

	} // loop

	Q_EMIT this->doLevelStartCountdown();

} // onPauseResumeToggled


void Game::onSpeedChanged(const int iIndex) {

	this->ubSpeedIndex = iIndex;

	int iInterval;
	switch (iIndex) {

		// beginner
		case 0: iInterval = 387; break;
			// slow
		case 1: iInterval = 81; break;
			// medium
		case 2: iInterval = 52; break;
			// fast
		case 3: iInterval = 21; break;
			// inhumane
		default: iInterval = 0; break;

	} // switch iIndex

//	bool bRunning = this->pTimer->isActive();
//	this->pTimer->stop();

	this->pTimer->setInterval(iInterval);

//	if (bRunning) this->pTimer->start();

} // onSpeedChanged


void Game::onTick() {

	Q_EMIT this->move();
	Q_EMIT this->placeBonus(100u);

} // onTick


void Game::onWormAteBonus(Worm *pWorm, SurfaceCell *pCell) {

	//this->onDebugMessage("Worm ate bonus");
	// which one is it? how many points or other actions?

	// first find the bonus in our cache
	quint8 ubState = 0u;
	Bonus *pBonus;
	for (int i = 0; i < this->apBonus.length(); ++i) {

		pBonus = this->apBonus.at(i);
		if (!pBonus->contains(pCell)) continue;

		// found it, retrieve state
		ubState = pBonus->getStateBase();

	} // loop

	// didn't find any?
	if (0u == ubState) {
		this->onDebugMessage("could not find eaten bonus in Game::onWormAteBonus");
		return;
	} // if none found

	// OK, found it and have state
	// so tell good-bye
	pBonus->onGotEaten();

	// destroy it and remove it from cache
	this->destroyBonus(pBonus);

	// now let's react to it with points and growth
	switch (ubState) {

		case 100u: // apple

			// add points
			pWorm->onAddScore(SssS_Nibblers_Bonus_Apple_Points
							  * this->fFactorApple);
			// grow
			pWorm->onGrow(SssS_Nibblers_Bonus_Apple_Grow
						  + this->fFractionApple);

			this->fFactorApple += 0.1f;
			this->fFractionApple += 0.1f;

			if (0u == this->ubCountApplesLeft) {

				this->onDebugMessage("why? Game::onWormAteBonus");

			} else {

				this->ubCountApplesLeft--;
				if (0u == this->ubCountApplesLeft) {
					// TODO: level done
					//Q_EMIT this->
				} else {
					// TODO: keep track of apples as there may not be any space
					// but there needs to be one as soon as space is available
					Q_EMIT this->placeBonus(100u);
				} // if have apples or not

			} // if can subtract at all

		break;

		case 110u: // cherry

			// add points
			pWorm->onAddScore(SssS_Nibblers_Bonus_Cherry_Points
							  * this->fFactorCherry);
			// grow
			if (this->fFractionCherry < SssS_Nibblers_Bonus_Cherry_Grow) {

				pWorm->onGrow(SssS_Nibblers_Bonus_Cherry_Grow
							  - this->fFractionCherry);

			} else pWorm->onGrow(SssS_Nibblers_Bonus_Cherry_Grow);

			this->fFactorCherry += 0.1f;
			if (0.1f <= this->fFractionCherry) this->fFractionCherry -= 0.1f;

		break;

		case 120u: // banana

			// add points
			pWorm->onAddScore(SssS_Nibblers_Bonus_Banana_Points
							  * this->fFactorBanana);
			// grow
			pWorm->onGrow(SssS_Nibblers_Bonus_Banana_Grow
						  + this->fFractionBanana);

			this->fFactorBanana += 0.1f;
			this->fFractionBanana += 0.1f;

		break;

		case 130u: // heart

			// no points
			// no growth

			pWorm->onAddLife();

		break;

		case 140u: // diamond

			// no points
			// no growth

			for (int i = 0; i < this->apWorms.length(); ++i) {

				// only other worms
				if (pWorm == this->apWorms.at(i)) continue;

				// flip direction
				this->apWorms.at(i)->onReverse();

			} // loop worms

		break;

		default:
			this->onDebugMessage("unknown bonus encountered in Game::onWormAteBonus "
								 + QString::number(ubState));
		break;

	} // switch ubState

} // onWormAteBonus


void Game::onWormCrashed(Worm *pWorm) {

	pWorm->onSubtractLife();

	if (!pWorm->isDead()) Q_EMIT this->spawnWorm(pWorm);

} // onWormCrashed


void Game::onWormCreated(Worm *pWorm) {

	this->apWorms.append(pWorm);

	connect(pWorm, SIGNAL(died()),
			this, SLOT(onWormDied()));

} // onWormCreated


void Game::onWormDied() {

	++this->ubCountDead;
	if (this->isGameOver()) {

		this->pTimer->stop();

		Q_EMIT this->doGameOver();

	} // if all have died

} // onWormDied



}	} // namespace SwissalpS::QtNibblers

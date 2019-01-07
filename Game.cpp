#include "Game.h"



namespace SwissalpS { namespace QtNibblers {



Game::Game(QObject *pParent) :
	QObject(pParent),
	bLevelStarted(false),
	bPaused(true),
	ubCountDead(0u),
	pAS(AppSettings::pAppSettings()) {

	this->apWorms.clear();

} // construct


Game::~Game() {

	this->apWorms.clear();

} // dealloc


void Game::init() {

	this->pTimer = new QTimer(this);
	this->pTimer->setSingleShot(false);
	this->onSpeedChanged(this->pAS->get(AppSettings::sSettingGameSpeed).toInt());

	connect(this->pTimer, SIGNAL(timeout()),
			this, SLOT(onTick()));

} // init


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

} // onTick


void Game::onWormAteBonus(Worm *pWorm, const quint8 ubBonus) {

	this->onDebugMessage("Worm ate bonus");

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

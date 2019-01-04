#include "Game.h"



namespace SwissalpS { namespace QtNibblers {



Game::Game(QObject *pParent) :
	QObject(pParent),
	bLevelStarted(false),
	bPaused(true),
	pAS(AppSettings::pAppSettings()) {

	this->apWorms.clear();

} // construct


Game::~Game() {

	this->apWorms.clear();

} // dealloc


void Game::init() {

	this->pTimer = new QTimer(this);
	this->pTimer->setInterval(50);
	this->pTimer->setSingleShot(false);

	connect(this->pTimer, SIGNAL(timeout()),
			this, SLOT(onTick()));

} // init


void Game::onPauseResumeToggled() {

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



}	} // namespace SwissalpS::QtNibblers

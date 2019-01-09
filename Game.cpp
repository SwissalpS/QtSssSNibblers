#include "Game.h"

#include <QTime>



namespace SwissalpS { namespace QtNibblers {



Game::Game(QObject *pParent) :
	QObject(pParent),
	bLevelStarted(false),
	bPaused(true),
	bUseFakes(false),
	ubCountApplesLeft(13u),
	ubCountBonus(0u),
	ubCountBonusLeft(0u),
	ubCountBonusMissed(0u),
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

	// init randomizer
	qsrand(QTime::currentTime().msecsSinceStartOfDay());

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


void Game::addBonus(const bool bApple) {
	// don't always create bonus if it's not an apple
	if (!bApple) {

		if (this->ubCountBonusMissed >= SssS_Nibblers_Bonus_Max_Missed) return;

		if ((qrand() % 51) != 0) return;

		bool bMakeFake = ((qrand() % 8) != 0);

		if (bMakeFake && !this->bUseFakes) return;

		switch (qrand() % 22) {

			case 0: case 1: case 2: case 3: case 4:
			case 5: case 6: case 7: case 8: case 9:
				Q_EMIT this->placeBonus(L::BonusCherry, bMakeFake);
			break;

			case 10: case 11: case 12: case 13: case 14:
				Q_EMIT this->placeBonus(L::BonusBanana, bMakeFake);
			break;

			case 15:
				Q_EMIT this->placeBonus(L::BonusHeart, bMakeFake);
			break;

			default:
				if (1 < this->apWorms.length())
					Q_EMIT this->placeBonus(L::BonusDiamond, bMakeFake);
			break;

		} // switch qrand() % 22

	} else {

		// regular apple

		// also add a fake?
		if (this->bUseFakes	&& ((qrand() % 8) == 0)) {

			Q_EMIT this->placeBonus(L::BonusApple, true);

		} // if add a fake

		Q_EMIT this->placeBonus(L::BonusApple, false);

	} // if not apple

} // addBonus


void Game::destroyBonus(Bonus *pBonus) {

	this->disconnect(pBonus);
	this->apBonus.removeOne(pBonus);
	pBonus->deleteLater();

} // destroyBonus


void Game::init() {

	this->pTimer = new QTimer(this);
	this->pTimer->setSingleShot(false);

	connect(this->pTimer, SIGNAL(timeout()),
			this, SLOT(onTick()));

	this->pTimerBonus = new QTimer(this);
	this->pTimerBonus->setSingleShot(false);

	connect(this->pTimerBonus, SIGNAL(timeout()),
			this, SLOT(onTickBonus()));

	this->onSpeedChanged(this->pAS->get(AppSettings::sSettingGameSpeed).toInt());

} // init


void Game::onBonusPlaced(const QVector<SurfaceCell *> apCells, const bool bFake) {

	Bonus *pBonus = new Bonus(apCells, bFake, this);

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

		case L::BonusApple: // 100

			uiTicks = SssS_Nibblers_Bonus_Apple_Lifetime;

		break;

		case L::BonusCherry: // 110

			uiTicks = SssS_Nibblers_Bonus_Cherry_Lifetime;

		break;

		case L::BonusBanana: // 120

			uiTicks = SssS_Nibblers_Bonus_Banana_Lifetime;

		break;

		case L::BonusHeart: // 130

			uiTicks = SssS_Nibblers_Bonus_Heart_Lifetime;

		break;

		case L::BonusDiamond: // 140

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

	bool bWasApple = L::BonusApple == pBonus->getStateBase();
	if (!bWasApple) this->ubCountBonusMissed++;

	if (pBonus->isFake()) bWasApple = false;

	// TODO:
	// what kind? do we need to place it again?

	qint16 iPenalty = -1 * SssS_Nibblers_Bonus_Penalty_Miss
					  * this->ubCountLevels	* this->ubSpeedIndex;

	for (int i = 0; i < this->apWorms.length(); ++i) {

		this->apWorms.at(i)->onAddScore(iPenalty);

	} // loop worms

	// destroy Bonus
	this->destroyBonus(pBonus);

	if (bWasApple) this->addBonus(true);

} // onBonusTimedOut


void Game::onNoSpaceFoundForBonus(const quint8 ubBonus, const bool bFake) {
	Q_UNUSED(ubBonus)
	Q_UNUSED(bFake)

	// TODO:

} // onNoSpaceFoundForBonus


void Game::onPlayerCountChanged(const quint8 ubCountHumans,
								const quint8 ubCountAIs) {
	Q_UNUSED(ubCountAIs)
	Q_UNUSED(ubCountHumans)

	// TODO:

} // onPlayerCountChanged


void Game::onReset() {

	this->pTimer->stop();
	this->pTimerBonus->stop();

	this->onSpeedChanged(this->pAS->get(AppSettings::sSettingGameSpeed).toUInt());

	this->apBonus.clear();
	this->ubCountBonus = 8 + this->apWorms.length();
	this->ubCountApplesLeft = this->ubCountBonus;
	this->ubCountBonusMissed = 0;

	this->ubCountDead = 0u;
	this->ubCountLevels = 1u;
	this->bLevelStarted = false;

	this->bUseFakes = this->pAS->get(AppSettings::sSettingGameFakeBonuses).toBool();

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
			this->pTimerBonus->stop();

		} else {

			// do whatever to resume game

			this->pTimer->start();
			this->pTimerBonus->start();

		} // if pause or resume game

		return;

	} // if level already started

	// not yet started
	this->onReset();
	this->ubCountLevels = 1u;
	this->bLevelStarted = true;
	this->bPaused = true;

	Q_EMIT this->placeBonus(L::BonusApple, false);

	for (int iCount = 0; iCount < this->apWorms.length(); ++iCount) {

		Q_EMIT this->spawnWorm(this->apWorms.at(iCount));

	} // loop

	Q_EMIT this->doLevelStartCountdown();

} // onPauseResumeToggled


void Game::onSpeedChanged(const int iIndex) {

	this->ubSpeedIndex = iIndex;

	int iInterval;
	int iIntervalBonus;
	quint8 ubFactor;
	switch (iIndex) {

		// beginner
		case 0: ubFactor = 4; break;
			// slow
		case 1: ubFactor = 3; break;
			// medium
		case 2: ubFactor = 2; break;
			// fast
		case 3: ubFactor = 1; break;
			// inhumane
		default: ubFactor = 0; break;

	} // switch iIndex

	iInterval = ubFactor * SssS_Nibblers_Speed_Base;
	iIntervalBonus = qMax(ubFactor * SssS_Nibblers_Speed_Bonus_Base,
						  SssS_Nibblers_Speed_Bonus_Base);

	this->pTimer->setInterval(iInterval);
	this->pTimerBonus->setInterval(iIntervalBonus);

} // onSpeedChanged


void Game::onTick() {

	Q_EMIT this->move();

} // onTick


void Game::onTickBonus() {

	this->addBonus(false);

} // onTickBonus


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

	if (pBonus->isFake()) {

		pWorm->onReverse();
		this->destroyBonus(pBonus);
		return;

	} // if it's a fake

	// destroy it and remove it from cache
	this->destroyBonus(pBonus);

	this->onDebugMessage("cB " + QString::number(this->ubCountBonus)
						 + " cAl " + QString::number(this->ubCountApplesLeft));
	// now let's react to it with points and growth
	switch (ubState) {

		case L::BonusApple:

			// add points
			pWorm->onAddScore((this->ubCountBonus - this->ubCountApplesLeft)
							  * this->ubCountLevels);
			// grow
			pWorm->addLength(4 * (this->ubCountBonus - this->ubCountApplesLeft));

			this->fFactorApple += 0.1f;
			this->fFractionApple += 0.1f;

			if (0u == this->ubCountApplesLeft) {

				this->onDebugMessage("failed to detect 'level done' Game::onWormAteBonus");

			} else {

				this->ubCountApplesLeft--;
				if (0u == this->ubCountApplesLeft) {

					this->pTimer->stop();
					this->pTimerBonus->stop();
					this->bLevelStarted = false;
					Q_EMIT this->nextLevel();

				} else {
					// TODO: keep track of apples as there may not be any space
					// but there needs to be one as soon as space is available
					Q_EMIT this->placeBonus(L::BonusApple, false);
				} // if have apples or not

			} // if can subtract at all

		break;

		case L::BonusCherry:

			if (2 >= pWorm->targetLength()) break;

			// add points
			pWorm->onAddScore(0.5 * this->ubCountLevels * pWorm->targetLength());
			// grow
			pWorm->addLength(-0.5 * pWorm->targetLength());

			this->fFactorCherry += 0.1f;
			if (0.1f <= this->fFractionCherry) this->fFractionCherry -= 0.1f;

		break;

		case L::BonusBanana:

			// add points
			pWorm->onAddScore(pWorm->targetLength() * this->ubCountLevels);
			// grow
			pWorm->addLength(pWorm->targetLength());

			this->fFactorBanana += 0.1f;
			this->fFractionBanana += 0.1f;

		break;

		case L::BonusHeart:

			// no points
			// no growth

			pWorm->onAddLife();

		break;

		case L::BonusDiamond:

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

	// TODO: count dead humans if any human players...

	++this->ubCountDead;
	if (this->isGameOver()) {

		this->pTimer->stop();

		Q_EMIT this->doGameOver();

	} // if all have died

} // onWormDied



}	} // namespace SwissalpS::QtNibblers

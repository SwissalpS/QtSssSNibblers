/*
 * QtSssSNibblers: SwissalpS Nibbles written with Qt-Framework
 * Copyright (C) 2018-2019 Luke J. Zimmermann aka SwissalpS <SwissalpS@LukeZimmermann.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Game.h"

#include "Fx.h"
#include "IconEngine.h"

#include <QTime>



namespace SwissalpS { namespace QtNibblers {



Game::Game(QObject *pParent) :
	QObject(pParent),
	bGameStarted(false),
	bLevelStarted(false),
	bPaused(true),
	bUseFakes(false),
	ubCountAllPlayers(0u),
	ubCountApplesLeft(13u),
	ubCountBonus(0u),
	ubCountBonusLeft(0u),
	ubCountBonusMissed(0u),
	ubCountDead(0u),
	ubCountDeadHumans(0u),
	ubCountHumans(0u),
	ubCountLevels(0u),
	ubCountNeedApple(SssS_Nibblers_Bonus_Delay_Ticks),
	ubCurrentLevel(0u),
	ubSpeedIndex(0u),
	ubStartLevel(0u),
	pAS(AppSettings::pAppSettings()),
	pMapGame(nullptr),
	pTimer(nullptr),
	pTimerBonus(nullptr),
	pWormAI(nullptr) {

	this->apBonus.clear();
	this->apWorms.clear();

	// init 'AI'
	this->pWormAI = new WormAI(this);

	// init randomizer
	qsrand(uint(QTime::currentTime().msecsSinceStartOfDay()));

} // construct


Game::~Game() {

	this->destructBonuses();
	this->destructWorms();

	this->pAS = nullptr;

	delete this->pMapGame;
	this->pMapGame = nullptr;

	if (this->pTimer) {
		this->pTimer->stop();
		delete this->pTimer;
		this->pTimer = nullptr;
	}

	if (this->pTimerBonus) {
		this->pTimerBonus->stop();
		delete this->pTimerBonus;
		this->pTimerBonus = nullptr;
	}

	delete this->pWormAI;
	this->pWormAI = nullptr;

} // dealloc

// slightly adapted from nibbles-game.vala NibblesGame.add_bonus(bool)
// chances of each should be compareable to gnome-nibbles and it's ancestors.
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
				this->placeBonus(L::BonusCherry, bMakeFake);
			break;

			case 10: case 11: case 12: case 13: case 14:
				this->placeBonus(L::BonusBanana, bMakeFake);
			break;

			case 15:
				this->placeBonus(L::BonusHeart, bMakeFake);
			break;

			default:
				if (1 < this->apWorms.length())
					this->placeBonus(L::BonusDiamond, bMakeFake);
			break;

		} // switch qrand() % 22

	} else {

		// regular apple

		// also add a fake?
		if (this->bUseFakes	&& ((qrand() % 8) == 0)) {

			this->placeBonus(L::BonusApple, true);

		} // if add a fake

		// dirty fix to prevent more than one real apple at a time
		Bonus *pBonus;
		bool bHaveApple = false;
		for (int i = 0; i < this->apBonus.length(); ++i) {

			pBonus = this->apBonus.at(i);

			if (pBonus->isFake()) continue;

			if (pBonus->getStateBase() == L::BonusApple) {

				this->onDebugMessage("wanted to place an apple when there was one already!!!!!!");
				bHaveApple = true;
				break;

			} // if found real apple

		} // loop bonuses

		if (!bHaveApple) this->placeBonus(L::BonusApple, false);

	} // if not apple

} // addBonus


// static
void Game::addCrashPotential(QHash<QString, Worm *> &hppCrashPotential,
							 QVector<Worm *> &apCrashedWorms,
							 QPoint oPoint, Worm *pWorm) {

	QString sKey = QString::number(oPoint.x()) + "_" + QString::number(oPoint.y());
	if (hppCrashPotential.keys().contains(sKey)) {

		// head-on-colision
		if (!pWorm->isImmune()) apCrashedWorms.append(pWorm);

		if (!apCrashedWorms.contains(hppCrashPotential.value(sKey))) {

			if (!hppCrashPotential.value(sKey)->isImmune())
				apCrashedWorms.append(hppCrashPotential.value(sKey));

		} // if other worm needs to be added too

		return;

	} // if point is already in hash

	hppCrashPotential.insert(sKey, pWorm);

} // addCrashPotential


void Game::destroyBonus(Bonus *pBonus) {

	//this->onDebugMessage("destroyBonus");

	pBonus->onGotEaten();

	this->disconnect(pBonus);
	this->apBonus.removeOne(pBonus);
	pBonus->deleteLater();

} // destroyBonus


void Game::destructBonuses() {

	//this->onDebugMessage("destructBonuses");

	Bonus *pBonus;
	while (this->apBonus.length()) {

		pBonus = this->apBonus.takeLast();
		delete pBonus;

	} // loop this->apBonus.length()

} // destructBonuses


void Game::destructWorms() {

	//this->onDebugMessage("destructWorms");

	Q_EMIT this->wormsInvalidated();

	Worm *pWorm;
	while (this->apWorms.length()) {

		pWorm = this->apWorms.takeLast();
		delete pWorm;

	} // loop this->apWorms.length()

} // destructWorms


void Game::gameDone(const bool bGameWon) {

	Fx::play(Fx::GameOver);

	this->pTimer->stop();
	this->pTimerBonus->stop();
	this->bPaused = true;

	this->ubCountDead = 0xFFu;
	this->ubCountDeadHumans = 0xFFu;

	QString sOut = bGameWon ? tr("Game Won") : tr("Game Over");
	sOut += "\n";

	QVector<Worm *> apRanks = this->makeRanking();

	Worm *pWorm;
	for (int i = 0; i < apRanks.length(); ++i) {

		pWorm = apRanks.at(i);

		// prepare output for Game Over cover
		sOut += QString::number(i + 1) + ". " + pWorm->name() + ": "
				+ QString::number(pWorm->score())
				//+ " " + QString::number(pWorm->livesLost())
				//+ " " + QString::number(pWorm->levelCount())
				+ (((i+1) < apRanks.length()) ? "\n" : "");

		if (pWorm->isAI()) continue;

		// inform History about the results
		Q_EMIT this->newHistoryItem(
					new HistoryItem(
						this->bUseFakes,
						pWorm->name(),
						this->ubCountAllPlayers - this->ubCountHumans,
						this->ubCountHumans, pWorm->levelCount(),
						this->ubStartLevel, pWorm->livesLost(),
						this->ubSpeedIndex, pWorm->score()));

	} // loop worms sorted by rank

	Q_EMIT this->updateHistory();
	Q_EMIT this->doGameOver(sOut);

} // gameDone


void Game::init() {

	this->onDebugMessage("init");

	this->pTimer = new QTimer(this);
	this->pTimer->setSingleShot(false);
	this->pTimer->setTimerType(Qt::PreciseTimer);

	connect(this->pTimer, SIGNAL(timeout()),
			this, SLOT(onTick()));

	this->pTimerBonus = new QTimer(this);
	this->pTimerBonus->setSingleShot(false);
	this->pTimerBonus->setTimerType(Qt::PreciseTimer);

	connect(this->pTimerBonus, SIGNAL(timeout()),
			this, SLOT(onTickBonus()));

	this->onSpeedChanged(this->pAS->get(AppSettings::sSettingGameSpeed).toInt());

	//this->loadCurrentLevel();
	this->onStartNewGame(quint8(this->pAS->get(
									AppSettings::sSettingGameStartLevel).toUInt()));

} // init


void Game::initWorms() {

	this->onDebugMessage("initWorms");

	this->destructWorms();

	if (nullptr == this->pMapGame) {
		this->onDebugMessage("initWorms no map!");
		return;
	} // if no map, should not happen

	quint8 ubColour;
	quint8 ubCount;
	quint8 ubLives = quint8(this->pAS->get(AppSettings::sSettingGameStartLives).toUInt());
	quint8 ubLivesMax = (this->pAS->get(AppSettings::sSettingGameLimitLives).toBool()) ? 0u : 2u * ubLives;

	// check that there are enough spawn points
	if (this->ubCountAllPlayers > this->pMapGame->spawnPoints().length()) {

		Q_EMIT this->statusMessage(tr("Level does not have sufficient spawn-points. Bailling."));
		Q_EMIT this->doLevelIsMissingSpawnPoints(
					this->ubCountAllPlayers
					- this->pMapGame->spawnPoints().length());

		this->ubCountDead = 0xFFu;
		this->ubCountDeadHumans = 0xFFu;

		delete this->pMapGame;
		this->pMapGame = nullptr;

		return;

	} // if not enough start points

	// random spawn point distribution
	this->pMapGame->randomizeSpawnOrder();

	QPoint oPoint;
	quint8 ubState;
	Worm *pWorm;
	QString sName;
	QStringList aUsedNames;
	for (ubCount = 0u; ubCount < this->ubCountAllPlayers; ++ubCount) {

		oPoint = this->pMapGame->spawnPoints().at(ubCount);
		ubState = this->pMapGame->tile(oPoint);

		ubColour = this->pAS->getPlayerColour(ubCount);

		pWorm = new Worm(oPoint, ubState, ubColour, (ubCount >= ubCountHumans), ubLivesMax, this);

		if (!pWorm->isAI())
			pWorm->setUseRelativeControls(this->pAS->getPlayerRelative(ubCount));

		connect(pWorm, SIGNAL(debugMessage(QString)),
				this, SLOT(onDebugMessage(QString)));

		connect(pWorm, SIGNAL(died(bool)),
				this, SLOT(onWormDied(bool)));

		connect(pWorm, SIGNAL(fart()),
				this, SLOT(onWormFarted()));

		this->apWorms.append(pWorm);

		Q_EMIT this->wormCreated(pWorm);

		pWorm->onSetLives(ubLives);

		if (pWorm->isAI()) {

			pWorm->onSetName(tr("AI ") + QString::number((ubCount + 1u) - ubCountHumans));

		} else {

			sName = this->pAS->getPlayerName(ubCount);

			// deal with empty names
			if (sName.isEmpty()) sName = tr("Player") + " "
										 + QString::number(ubCount + 1u);

			// deal with duplicate names
			while (aUsedNames.contains(sName)) sName += " I";
			aUsedNames << sName;

			pWorm->onSetName(sName);

		} // if AI or human

	} // loop worms

} // initWorms


bool Game::isGameOver() {

	// can't be over if it has not started
	if (!this->bGameStarted) return false;

	//this->onDebugMessage("isGameOver");

	bool bUseLastDeadMethod = this->pAS->get(
								  AppSettings::sSettingGameOverOnLastDead).toBool();

	if (this->ubCountHumans) {

		// got humans

		if (bUseLastDeadMethod) {

			// a la SwissalpS
			// last human dead -> game over
			return (this->ubCountDeadHumans >= this->ubCountHumans);

		} else {

			// a la gnome-nibbles
			// first human dead -> game over
			return (0 != this->ubCountDeadHumans);

		} // if using SwissalpS or gnome-nibbles way of detecting game over

	} else {

		// all AI
		// this is not possible with gnome-nibbles but we still offer the option

		if (bUseLastDeadMethod) {

			// a la SwissalpS
			// last dead -> game over
			return this->ubCountDead >= this->apWorms.length();

		} else {

			// a la gnome-nibbles
			// first dead -> game over
			return 0u != this->ubCountDead;

		} // if using SwissalpS or gnome-nibbles way of detecting game over

	} // if have humans at all or all AI

} // isGameOver


void Game::loadCurrentLevel() {

	this->onDebugMessage("loadCurrentLevel");

	bool bBadMap = true;
	quint8 ubFirstLevel = this->ubCurrentLevel;
	quint8 ubBMmode = quint8(this->pAS->get(AppSettings::sSettingGameBadLevelMode).toUInt());
	QString sMessage;
	QString sPath;

	//this->onDebugMessage("mode: " + QString::number(ubBMmode));

	while (bBadMap) {

		bBadMap = false;
		sPath = this->pAS->getDataPathLevelFile(this->ubCurrentLevel);

		delete this->pMapGame; this->pMapGame = nullptr;
		this->pMapGame = MapGame::loadedMap(sPath, this);
		if (MapGame::NoError != this->pMapGame->errorCode()) {

			this->onDebugMessage("Load Error");
			bBadMap = true;

			if (0u == ubBMmode) {

				if (this->ubStartLevel == this->ubCurrentLevel) {

					Q_EMIT this->doLevelLoadError();

					return;

				} // if first level

				// Game Won

				this->gameDone(true);

				return;

			} else if (1u == ubBMmode) {

				// loop back

				if (this->ubStartLevel == this->ubCurrentLevel) {

					// stuck -> Game Won

					Q_EMIT this->doLevelLoadError();
					return;

				} // if stuck

				this->ubCurrentLevel = this->ubStartLevel;

			} else {

				// skip to next good one

				if (0xFFu == this->ubCurrentLevel) this->ubCurrentLevel = 0u;
				else this->ubCurrentLevel++;

				if (ubFirstLevel == this->ubCurrentLevel) {

					// were around the world and didn't find a single good one
					// how sad

					this->gameDone(true);

					return;
					Q_EMIT this->doLevelLoadError();
					return;

				} // if round the world

			} // switch mode

			continue;

		} // if map had load issues

		// check that there are enough spawn points
		if (this->ubCountAllPlayers > this->pMapGame->spawnPoints().length()) {

			this->onDebugMessage("Too Few Spawn points");

			bBadMap = true;

			if (0u == ubBMmode) {

				if (this->ubStartLevel == this->ubCurrentLevel) {

					Q_EMIT this->doLevelIsMissingSpawnPoints(this->ubCountAllPlayers
															 - this->pMapGame->spawnPoints().length());

					return;

				}
				// Game Won
				this->onDebugMessage("Game Won");

				this->gameDone(true);

				Q_EMIT this->statusMessage(tr("Level does not have sufficient spawn-points. Bailling."));

				return;

			} else if (1u == ubBMmode) {

				// loop back
				this->onDebugMessage("Loop");

				if (this->ubStartLevel == this->ubCurrentLevel) {

					// stuck -> Game Won
					this->onDebugMessage("Game Won");

					Q_EMIT this->doLevelLoadError();

					return;

				} // if stuck

				this->ubCurrentLevel = this->ubStartLevel;

			} else {

				// skip to next good one
				this->onDebugMessage("skip");

				if (0xFFu == this->ubCurrentLevel) this->ubCurrentLevel = 0u;
				else this->ubCurrentLevel++;

				if (ubFirstLevel == this->ubCurrentLevel) {

					// were around the world and didn't find a single good one
					// how sad
					this->onDebugMessage("Around the world");

					this->gameDone(true);

					return;

					Q_EMIT this->doLevelLoadError();
					return;

				} // if round the world

			} // switch mode

		} // if not enough start points

	} // loop until good map found

	connect(this->pMapGame, SIGNAL(debugMessage(QString)),
			this, SLOT(onDebugMessage(QString)));


	Q_EMIT this->loadLevel(this->pMapGame, this->ubCurrentLevel);

} // loadCurrentLevel


QVector<Worm *> Game::makeRanking() {

	QVector<Worm *> apRest = this->apWorms;
	QVector<Worm *> apRanks;
	Worm *pWorm;
	Worm *pWormHighest;
	quint16 uiHighestScore;

	// sort by highest score
	while (apRest.length()) {

		uiHighestScore = 0;
		pWormHighest = apRest.first();
		for (int i = 0; i < apRest.length(); ++i) {

			pWorm = apRest.at(i);
			if (pWorm->score() > uiHighestScore) {

				uiHighestScore = pWorm->score();
				pWormHighest = pWorm;

			} // if found a higher score

		} // loop rest

		if (uiHighestScore) {

			// move worm into other array
			apRanks.append(pWormHighest);
			apRest.removeOne(pWormHighest);

		} else {

			// the rest have a Zero score
			apRanks += apRest;
			// clear so we break and are neat
			apRest.clear();

		} // if got highest or not

	} // loop apRest.length()

	// sort those with same scores by lives lost
	bool bFound = true;
	while (bFound) {

		bFound = false;
		apRest = apRanks;
		apRanks.clear();
		while (apRest.length()) {

			if (2 > apRest.length()) {
				// nothing to compare to
				apRanks += apRest;
				apRest.clear();
				break;
			} // if only one left

			pWorm = apRest.first();
			pWormHighest = apRest.at(1);

			if (pWorm->score() == pWormHighest->score()) {

				// found 2 with identical score

				if (pWorm->livesLost() > pWormHighest->livesLost()) {

					// first has lost more lives than next

					// mark that we need one more round
					bFound = true;

					apRanks.append(pWormHighest);
					apRest.removeAt(1);

				} else {

					// first is still better off

					apRanks.append(pWorm);
					apRest.removeFirst();

				} // if first is worse off than second

			} else {

				// scores differ -> use current order

				apRanks.append(pWorm);
				apRest.removeFirst();

			} // if found identical scores or not

		} // loop apRest.length()

	} // loop bFound

	return apRanks;

} // makeRanking


void Game::onBonusPlaced(const QVector<SurfaceCell *> apCells, const bool bFake) {

	Bonus *pBonus = new Bonus(apCells, bFake, this);

	//this->onDebugMessage("onBonusPlaced " + QString::number(pBonus->getStateBase()) + " " + QString::number(apCells.at(0)->getPos().x()) + ":" + QString::number(apCells.at(0)->getPos().y()) + " " + QString::number(bFake));

	this->apBonus.prepend(pBonus);

	if (L::BonusApple != pBonus->getStateBase()) Fx::play(Fx::Appear);

	// wich kind? how many ticks until expired?
	quint16 uiTicks = 0u;
	switch (pBonus->getStateBase()) {

		case L::BonusApple: // 100

			if (!bFake) this->ubCountNeedApple = 0u;

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

	connect(this, SIGNAL(move()),
			pBonus, SLOT(onTick()));

	connect(pBonus, SIGNAL(debugMessage(QString)),
			this, SLOT(onDebugMessage(QString)));

	connect(pBonus, SIGNAL(timedOut(Bonus*)),
			this, SLOT(onBonusTimedOut(Bonus*)));

} // onBonusPlaced


void Game::onBonusTimedOut(Bonus *pBonus) {

	//this->onDebugMessage("onBonusTimedOut" + QString::number(pBonus->getStateBase()) + " " + QString::number(pBonus->getCells().at(0)->getColumn()) + ":" + QString::number(pBonus->getCells().at(0)->getRow()) + " " + QString::number(pBonus->isFake()));

	bool bWasApple = L::BonusApple == pBonus->getStateBase();
	if (pBonus->isFake()) bWasApple = false;
	if (bWasApple) this->ubCountBonusMissed++;

	// destroy Bonus
	this->destroyBonus(pBonus);

	// spawn another if it was an apple
	if (bWasApple) this->addBonus(true);

} // onBonusTimedOut


void Game::onLevelIsLoaded() {

	this->onDebugMessage("onLevelIsLoaded");

	if (0 == this->apWorms.length()) {

		//this->onDebugMessage("no worms");

		this->initWorms();

		return;

	} // if new game

	// todo: do we still need this safe-guard?
	if (nullptr == this->pMapGame) {
		this->onDebugMessage("no map");
		return;
	}

//	this->onDebugMessage("really check again? didn't we check when loading?");

//	// check that there are enough spawn points
//	if (this->apWorms.length() > this->pMapGame->spawnPoints().length()) {

//		Q_EMIT this->statusMessage(tr("Level does not have sufficient spawn-points. Bailling."));

//		return;

//	} // if not enough start points

//	this->onDebugMessage("OK, got enough spawn points");

	// random spawn point distribution
	this->pMapGame->randomizeSpawnOrder();

	QPoint oPoint;
	quint8 ubState;
	quint8 ubCount;
	for (ubCount = 0u; ubCount < this->apWorms.length(); ++ubCount) {

		oPoint = this->pMapGame->spawnPoints().at(ubCount);
		ubState = this->pMapGame->tile(oPoint);

		this->apWorms.at(ubCount)->onSetSpawnPoint(oPoint, ubState);

	} // loop worms

	this->ubCountLevels++;

} // onLevelIsLoaded


void Game::onNextLevel() {

	this->onDebugMessage("onNextLevel");

	if (0xFFu == this->ubCurrentLevel) this->ubCurrentLevel = 0u;
	else this->ubCurrentLevel++;

	this->loadCurrentLevel();

} // onNextLevel


void Game::onPlayerCountChanged(const quint8 ubCountHumans,
								const quint8 ubCountAIs) {
	Q_UNUSED(ubCountAIs)
	Q_UNUSED(ubCountHumans)

	// nothing to do, covered when restarting game

} // onPlayerCountChanged


void Game::onPlayerNameChanged(const quint8 ubWorm, const QString sName) {

	// watchdog
	if (ubWorm >= this->apWorms.length()) return;

	this->apWorms.at(ubWorm)->onSetName(sName);

} // onPlayerNameChanged


void Game::onPauseResumeToggled() {

	this->onDebugMessage("onPRT bP: " + QString::number(this->bPaused)
						 + " bGO: " + QString::number(this->isGameOver())
						 + " bLS: " + QString::number(this->bLevelStarted)
						 + " bGS: " + QString::number(this->bGameStarted)
						 );

	if (this->isGameOver()) return;

	if (this->bLevelStarted) {

		this->bPaused = !this->bPaused;

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

	// has game started?
	if (this->bGameStarted) {
		// new level starts
		this->onResetSoft();
	} else {
		// new game starts
		this->onReset();
		this->bGameStarted = true;
	}
	this->bLevelStarted = true;

	Q_EMIT this->doLevelStartCountdown();

} // onPauseResumeToggled


// aka onResetLevel
void Game::onResetSoft() {

	this->onDebugMessage("onResetSoft " + QString::number(this->apWorms.length()));

	this->apBonus.clear();

	this->bLevelStarted = false;
	this->bPaused = true;

	this->pTimer->stop();
	this->pTimerBonus->stop();

	this->ubCountBonus = 8u + quint8(this->apWorms.length());
	this->ubCountApplesLeft = this->ubCountBonus;
	this->ubCountBonusMissed = 0;
	this->ubCountNeedApple = SssS_Nibblers_Bonus_Delay_Ticks;

	Worm *pWorm;
	for (int iCount = 0; iCount < this->apWorms.length(); ++iCount) {

		pWorm = this->apWorms.at(iCount);

		if (pWorm->isDead()) continue;

		Q_EMIT this->spawnWorm(pWorm);

	} // loop

} // onResetSoft


// aka onResetGame
void Game::onReset() {

	this->onDebugMessage("onReset");

	this->bUseFakes = this->pAS->get(AppSettings::sSettingGameFakeBonuses).toBool();

	this->onSpeedChanged(this->pAS->get(AppSettings::sSettingGameSpeed).toInt());

	this->ubCountDead = 0u;
	this->ubCountDeadHumans = 0u;
	this->ubCountLevels = 1u;
	this->bGameStarted = false;

	//this->initWorms();

	this->onResetSoft();

} // onReset


void Game::onSpeedChanged(const int iIndex) {

	//this->onDebugMessage("onSpeedChanged");

	this->ubSpeedIndex = quint8(iIndex);

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
	// avoid 0 interval as things need to be done too
	// 12 was the longest I measured a onTick() to take
	// however, at times, for a couple seconds, it can take
	// well over 40 ms to get back to the next call to onTick()
	// garbage collection? leak? what could be causing this temporary throtle?
	// it has nothing to do with looking for spots to put bonus or fading tiles
	// the first is included in the 12 ms and the second hadn't been added yet
	// when I did the measurements
	iInterval = qMax(iInterval, 12);

	// have at least a foctor of 1 for bonus
	ubFactor = qMax(ubFactor, quint8(1u));
	iIntervalBonus = ubFactor * SssS_Nibblers_Speed_Bonus_Base;

	this->pTimer->setInterval(iInterval);
	this->pTimerBonus->setInterval(iIntervalBonus);

} // onSpeedChanged


void Game::onStartNewGame(const quint8 ubLevel) {

	this->onDebugMessage("onStartNewGame");

	this->ubCurrentLevel = ubLevel;
	this->ubStartLevel = ubLevel;

	this->bGameStarted = false;
	this->bLevelStarted = false;

	this->destructWorms();

	quint8 ubCountAIs = quint8(this->pAS->get(AppSettings::sSettingGameCountAIs).toUInt());
	this->ubCountHumans = quint8(this->pAS->get(AppSettings::sSettingGameCountHumans).toUInt());
	this->ubCountAllPlayers = this->ubCountHumans + ubCountAIs;

	this->loadCurrentLevel();

} // onStartNewGame


void Game::onTileChanged(const QPoint oPoint, const quint8 ubState) {

	//this->onDebugMessage("onTC " + QString::number(oPoint.x()) + ":" + QString::number(oPoint.y()) + " " + QString::number(ubState));
	this->pMapGame->setTile(oPoint, ubState);

} // onTileChanged


void Game::onTick() {

	static QTime oTime;
	int iElapsedLast = oTime.elapsed();
	oTime.start();
	int iElapsed;
	static int iLongest = 0;

	//this->onDebugMessage("onTick");

	// penalty for having missed bonuses
	if (SssS_Nibblers_Bonus_Max_Missed < this->ubCountBonusMissed) {

		qint16 iPenalty = -1 * SssS_Nibblers_Bonus_Penalty_Miss;

		Worm *pWorm;
		for (int i = 0; i < this->apWorms.length(); ++i) {

			pWorm = this->apWorms.at(i);

			// no penalty for dead worms
			// gnome-nibbles does not regard this
			if (pWorm->isDead()) continue;

			pWorm->onAddScore(iPenalty);

		} // loop worms

	} // if penalty

	Q_EMIT this->move();

	static QVector<quint8> aStatesPickups;
	static QVector<quint8> aStatesSnakes;
	static QVector<quint8> aStatesSpawns;
	static QVector<quint8> aStatesTeleporterEntrances;
	static QVector<quint8> aStatesTeleporterExits;
	static QVector<quint8> aStatesWalls;
	if (aStatesPickups.isEmpty()) {

		aStatesPickups = IconEngine::statesPickups();
		aStatesSnakes = IconEngine::statesSnakes();
		aStatesSpawns = IconEngine::statesSpawns();
		aStatesTeleporterEntrances = IconEngine::statesTeleporterEntrances();
		aStatesTeleporterExits = IconEngine::statesTeleporterExits();
		aStatesWalls = IconEngine::statesWalls();

	} // if first call

	Worm *pWorm;
	QPoint oPoint;
	QPoint oPointTeleporter;
	quint8 ubState;
	// keep track of cells that are being newly occupied
	QHash<QString, Worm *> hopCrashPotential;
	// keep track of worms that are crashing
	QVector<Worm *> apCrashedWorms;
	hopCrashPotential.clear();
	apCrashedWorms.clear();

	// collect immediate threats and goodies
	for (int i = 0; i < this->apWorms.length(); ++i) {

		pWorm = this->apWorms.at(i);

		if (pWorm->isDead()) continue;

		oPoint = pWorm->nextPoint();
		ubState = this->pMapGame->tile(oPoint);

		if (pWorm->isImmune()) {

			// others may crash? Maybe better not
			//addCrashPotential(hppCrashPotential, apCrashedWorms, pCell, pWorm);

			// TODO: check bonus, not to give a spawning worm points,
			// but to make sure it is drawn again

			Q_EMIT this->advanceWormTo(pWorm, oPoint);

			continue;

		} // if immune worm

		if (aStatesSnakes.contains(ubState)
				|| aStatesWalls.contains(ubState)) {

			// crash
			apCrashedWorms.append(pWorm);
			continue;

		} // if crashed

		if (aStatesTeleporterEntrances.contains(ubState)) {

			// enter teleporter
			oPointTeleporter = this->pMapGame->teleporterOutForIn(ubState);
			if (0xFF > oPointTeleporter.x()) {

				Fx::play(Fx::Teleport);

				// add both entrance and exit to crash potentials
				Game::addCrashPotential(hopCrashPotential, apCrashedWorms,
								  oPoint, pWorm);
				Game::addCrashPotential(hopCrashPotential, apCrashedWorms,
								  oPointTeleporter, pWorm);

				// advance to both locations
				Q_EMIT this->advanceWormTo(pWorm, oPoint);
				Q_EMIT this->advanceWormTo(pWorm, oPointTeleporter);

				continue;

			} // if matching exit exists

		} // if teleporter entrance encountered

		if (aStatesPickups.contains(ubState)) {

			// advance worm
			Q_EMIT this->advanceWormTo(pWorm, oPoint);

			// figure out which bonus it is
			this->wormAteBonus(pWorm, oPoint);
			Q_EMIT this->wormAteBonus(pWorm);

			Game::addCrashPotential(hopCrashPotential,apCrashedWorms, oPoint, pWorm);

		} else {

			// any free cell
			Q_EMIT this->advanceWormTo(pWorm, oPoint);

			Game::addCrashPotential(hopCrashPotential, apCrashedWorms,
									oPoint, pWorm);

		} // if picked up something

	} // loop

	// deal with crashed worms
	for (int i = 0; i < apCrashedWorms.length(); ++i) {

		Fx::play(Fx::Crash);

		pWorm = apCrashedWorms.at(i);

		Q_EMIT this->wormCrashed(pWorm);

		pWorm->onSubtractLife();

		if (1 < this->apWorms.length()) {

			pWorm->onMultiplyScore(0.7f);

		} // if more than one player -> penalty points

		if (!pWorm->isDead()) Q_EMIT this->spawnWorm(pWorm);

	} // loop crashed worms

	// do AI-moves
	for (int i = 0; i < this->apWorms.length(); ++i) {

		pWorm = this->apWorms.at(i);

		if (!pWorm->isAI()) continue;

		if (pWorm->isDead()) continue;

		this->pWormAI->move(pWorm, this->apWorms, this->pMapGame);

	} // loop worms

	iElapsed = oTime.elapsed();
	//this->onDebugMessage(QString::number(iElapsedLast) + " :since last | used ms: " + QString::number(iElapsed) + " longest: " + QString::number(iLongest));
	if (iElapsed > iLongest) iLongest = iElapsed;

} // onTick


void Game::onTickBonus() {

	//this->onDebugMessage("onTickBonus " + QString::number(this->ubCountNeedApple));

	// give time for space to appear for bonus apples
	// which really need to be in existance or game stalls
	if (this->ubCountNeedApple) {
		this->ubCountNeedApple--;
		if (0u == this->ubCountNeedApple) this->addBonus(true);
	} // if counting down for next apple

	// randomly add any other kind of bonus
	this->addBonus(false);

} // onTickBonus


void Game::onWormDied(const bool bAI) {

	this->onDebugMessage("onWormDied " + QString::number(bAI));

	++this->ubCountDead;
	if (!bAI) ++this->ubCountDeadHumans;

	// if all have died
	if (this->isGameOver()) this->gameDone(false);

} // onWormDied


void Game::placeBonus(const quint8 ubBonus, const bool bFake) {

	//this->onDebugMessage("placeBonus");

	// find 4 adjacent cells that are not occupied
	QVector<QPoint>aoPoints = this->pMapGame->freeSpotForBonus();
	if (aoPoints.isEmpty()) {

		if ((!bFake) && (L::BonusApple == ubBonus)) {

			this->onDebugMessage("Could not find spot for bonus");

			this->ubCountNeedApple = SssS_Nibblers_Bonus_Delay_Ticks;

		} // if not a fake apple

		return;

	} // if no space for bonus

	Q_EMIT this->bonusPlaced(aoPoints, ubBonus, bFake);

} // placeBonus


void Game::wormAteBonus(Worm *pWorm, const QPoint oPoint) {

	//this->onDebugMessage("wormAteBonus");

	// which one is it? how many points or other actions?

	// first find the bonus in our cache
	quint8 ubState = 0u;
	Bonus *pBonus = nullptr;
	for (int i = 0; i < this->apBonus.length(); ++i) {

		pBonus = this->apBonus.at(i);

		if (!pBonus->contains(oPoint)) continue;

		// found it, retrieve state
		ubState = pBonus->getStateBase();

		break;

	} // loop

	// didn't find any?
	if (0u == ubState) {
		this->onDebugMessage("could not find eaten bonus in Game::wormAteBonus");
		return;
	} // if none found

	//this->onDebugMessage("Worm ate bonus " + QString::number(ubState) + " " + QString::number(oPoint.x()) + ":" + QString::number(oPoint.y()));

	// OK, found it and have state
	// so tell good-bye
	pBonus->onGotEaten();

	if (pBonus->isFake()) {

		Fx::play(Fx::Reverse);

		pWorm->onReverse();
		this->destroyBonus(pBonus);
		return;

	} // if it's a fake

	// destroy it and remove it from cache
	this->destroyBonus(pBonus);

	// now let's react to it with points and growth
	quint16 uiScore;
	switch (ubState) {

		case L::BonusApple:

			Fx::play(Fx::Gobble);

			//this->onDebugMessage("cB " + QString::number(this->ubCountBonus) + " cAl " + QString::number(this->ubCountApplesLeft));

			// add points
			uiScore = qMax(1, (this->ubCountBonus - this->ubCountApplesLeft)
						   * this->ubCountLevels);
			pWorm->onAddScore(uiScore);
			// grow
			pWorm->addLength(4 * (this->ubCountBonus - this->ubCountApplesLeft));

			if (0u == this->ubCountApplesLeft) {

				this->onDebugMessage("failed to detect 'level done' Game::wormAteBonus");

			} else {

				this->ubCountApplesLeft--;
				if (0u == this->ubCountApplesLeft) {

					this->bPaused = true;
					this->pTimer->stop();
					this->pTimerBonus->stop();
					this->bLevelStarted = false;

					for (int i = 0; i < this->apWorms.length(); ++i) {

						if (!this->apWorms.at(i)->isDead()) this->apWorms.at(i)->onAddLevel();

					} // loop worms

					this->onDebugMessage("wormAteBonus:level done");
					Q_EMIT this->doLevelDone();

				} else {

					this->addBonus(true);

				} // if have apples or not

			} // if can subtract at all

		break;

		case L::BonusCherry:

			Fx::play(Fx::Bonus);

			if (2 >= pWorm->targetLength()) break;

			// add points
			uiScore = qMax(qint16(1), qint16(double(0.5) * double(this->ubCountLevels)
									 * double(pWorm->targetLength())));
			pWorm->onAddScore(uiScore);

			// grow
			pWorm->addLength(int(double(-0.5) * double(pWorm->targetLength())));


		break;

		case L::BonusBanana:

			Fx::play(Fx::Bonus);

			// add points
			uiScore = qMax(qint16(1), qint16(pWorm->targetLength() * this->ubCountLevels));
			pWorm->onAddScore(uiScore);

			// grow
			pWorm->addLength(pWorm->targetLength());

		break;

		case L::BonusHeart:

			Fx::play(Fx::Life);

			// no points
			// no growth

			pWorm->onAddLife();

		break;

		case L::BonusDiamond:

			Fx::play(Fx::Reverse);

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
			this->onDebugMessage("unknown bonus encountered in Game::wormAteBonus "
								 + QString::number(ubState));
		break;

	} // switch ubState

} // wormAteBonus


}	} // namespace SwissalpS::QtNibblers

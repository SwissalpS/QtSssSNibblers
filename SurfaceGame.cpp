#include "SurfaceGame.h"
#include "ui_SurfaceGame.h"

#include "definitions.h"
#include "IconEngine.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QTime>
#include <QTimer>
#include <QVBoxLayout>



namespace SwissalpS { namespace QtNibblers {



SurfaceGame::SurfaceGame(QWidget *pParent) :
	QFrame(pParent),
	pUi(new Ui::SurfaceGame),
	bProtectPP(false),
	pAS(AppSettings::pAppSettings()),
	pDialogLoad(nullptr),
	pStartCountDownFrame(nullptr),
	ubCurrentLevel(0xFFu),
	ubAIcountDeadendRun(1u) {

	this->pUi->setupUi(this);

	this->pAImap = new Map(SssS_Nibblers_Surface_Width,
						   SssS_Nibblers_Surface_Height, this);

	// init randomizer
	qsrand(uint(QTime::currentTime().msecsSinceStartOfDay()));

	// for forced aspect ratio
	this->iLastHeight = height();

	// for key events
	setFocusPolicy(Qt::StrongFocus);

	this->ahKeys.clear();
	this->aopRows.clear();
	this->apScoreBoards.clear();
	this->apWorms.clear();
	this->apSpawnPoints.clear();
	this->hpTeleporterEntrances.clear();
	this->hpTeleporterExits.clear();

	this->ubCurrentLevel = quint8(this->pAS->get(
							   AppSettings::sSettingGameStartLevel).toUInt());

} // construct


SurfaceGame::~SurfaceGame() {

	this->aopRows.clear();
	this->apScoreBoards.clear();
	this->apWorms.clear();
	this->apSpawnPoints.clear();
	this->hpTeleporterEntrances.clear();
	this->hpTeleporterExits.clear();

	delete this->pUi;

} // dealloc


void SurfaceGame::addCrashPotential(QHash<SurfaceCell *, Worm *> &hppCrashPotential,
									QVector<Worm *> &apCrashedWorms,
									SurfaceCell *pCell, Worm *pWorm) {

	if (hppCrashPotential.keys().contains(pCell)) {

		// head-on-colision
		if (!pWorm->isImmune()) apCrashedWorms.append(pWorm);

		if (!apCrashedWorms.contains(hppCrashPotential.value(pCell))) {

			if (!hppCrashPotential.value(pCell)->isImmune())
				apCrashedWorms.append(hppCrashPotential.value(pCell));

		} // if other worm needs to be added too

		return;

	} // if cell is already in hash

	hppCrashPotential.insert(pCell, pWorm);

} // addCrashPotential


void SurfaceGame::changeEvent(QEvent *pEvent) {

	QFrame::changeEvent(pEvent);

	switch (pEvent->type()) {

		case QEvent::LanguageChange:
			this->pUi->retranslateUi(this);
		break;

		default:
		break;

	} // switch

} // changeEvent


void SurfaceGame::clearSurface() {

	this->apSpawnPoints.clear();
	this->hpTeleporterEntrances.clear();
	this->hpTeleporterExits.clear();

	quint8 ubRows = this->aopRows.count();
	quint8 ubColumns = this->aopRows.first().count();
	quint8 ubX = 0u;
	quint8 ubY = 0u;
	QList<SurfaceCell*> aRow;
	SurfaceCell *pCell;

	for (ubY = 0u; ubY < ubRows; ubY++) {

		aRow = this->aopRows.at(ubY);

		for (ubX = 0u; ubX < ubColumns; ubX++) {

			pCell = aRow.at(ubX);
			this->setCellState(pCell, L::FloorClean, false);

		} // loop columns

	} // loop rows

	this->update();

} // clearSurface


void SurfaceGame::clearSurfaceOf(const quint8 ubState) {

	QVector<quint8> aStates;
	aStates.append(ubState);

	this->clearSurfaceOf(aStates);

} // clearSurfaceOf(single)


void SurfaceGame::clearSurfaceOf(const QVector<quint8> aStates) {

	quint8 ubRows = this->aopRows.count();
	quint8 ubColumns = this->aopRows.first().count();
	quint8 ubX = 0u;
	quint8 ubY = 0u;
	QList<SurfaceCell*> aRow;
	SurfaceCell *pCell;

	for (ubY = 0u; ubY < ubRows; ubY++) {

		aRow = this->aopRows.at(ubY);

		for (ubX = 0u; ubX < ubColumns; ubX++) {

			pCell = aRow.at(ubX);

			if (aStates.contains(pCell->getState())) {

				this->setCellState(pCell, L::FloorClean, false);

			} // if one to clear

		} // loop columns

	} // loop rows

	this->update();

} // clearSurfaceOf(vector)


void SurfaceGame::clearSurfaceOfWorm(const quint8 ubWormColourIndex) {

	this->clearSurfaceOf(IconEngine::statesSnake(ubWormColourIndex));

} // clearSurfaceOfWorm


void SurfaceGame::clearSurfaceOfWorms() {

	this->clearSurfaceOf(IconEngine::statesSnakes());

} // clearSurfaceOfWorms


Map *SurfaceGame::currentMap() {

	quint8 ubRows = this->aopRows.count();
	quint8 ubColumns = this->aopRows.first().count();
	quint8 ubX = 0u;
	quint8 ubY = 0u;

	Map *pMap = new Map(ubColumns, ubRows, this);

	for (ubY = 0u; ubY < ubRows; ubY++) {

		for (ubX = 0u; ubX < ubColumns; ubX++) {

			pMap->setTile(ubX, ubY, this->aopRows.at(ubY).at(ubX)->getState());

		} // loop columns

	} // loop rows

	return pMap;

} // currentMap


void SurfaceGame::focusInEvent(QFocusEvent *pEvent) {

	//this->onDebugMessage("todo: focus in");

	QFrame::focusInEvent(pEvent);

} // focusInEvent


void SurfaceGame::focusOutEvent(QFocusEvent *pEvent) {

	if (this->pUi->buttonPP->hasFocus()) return;
	if (this->pStartCountDownFrame->isVisible()) return;

	this->pauseIfRunning();

	QFrame::focusOutEvent(pEvent);

} // focusOutEvent


void SurfaceGame::countdownTick() {

	int iTick = this->pUi->buttonPP->text().toInt() - 1;

	if (0 < iTick) {

		QString sMessage = QString::number(iTick);
		this->pStartCountDownFrame->onSetText(sMessage);
		this->pStartCountDownFrame->update();
		this->pUi->buttonPP->setText(sMessage);
		Q_EMIT this->statusMessage(sMessage);

		QTimer::singleShot(1000, this, SLOT(countdownTick()));

		return;

	} // if still got ticks to go

	this->pStartCountDownFrame->hide();
	this->pUi->buttonPP->setChecked(true);
	this->pUi->buttonPP->setEnabled(true);
	this->pUi->buttonSR->setEnabled(true);
	this->pUi->buttonPP->setText(tr("Pause"));
	Q_EMIT this->statusMessage(tr("Go, Go, Goooooh!"));

	Q_EMIT this->pauseResumeToggled();

} // countdownTick


void SurfaceGame::dialogLoadFinished(const int iResult) {

	// canceled
	if (0 == iResult) return;

	this->ubCurrentLevel = this->pDialogLoad->getSelected();

	// TODO: make checkbox in settings for this feature
	this->pAS->setValue(AppSettings::sSettingGameStartLevel, this->ubCurrentLevel);

	this->loadCurrentLevel();

	Q_EMIT this->resetGame();

	this->resetButtons();

} // dialogLoadFinished


quint16 SurfaceGame::findNextGood(const QPoint oStart, const L::Heading eDirection) {

	static QVector<quint8> aStatesPickups = IconEngine::statesPickups();
	//static QVector<quint8> aStatesTeleporterEntrances = IconEngine::statesTeleporterEntrances();
	//static QVector<quint8> aStatesTeleporterExits = IconEngine::statesTeleporterExits();
	static QVector<quint8> aStatesToAvoid =
			IconEngine::statesSnakes() + IconEngine::statesWalls();

	SurfaceCell *pCell;

	quint8 ubState;
	quint16 uiDistance = 0u;
	QPoint oNext(oStart);

	while (0xFFFFu > uiDistance) {

		pCell = this->getCell(oNext);
		if (pCell->isNull()) return 0xFFFFu;

		ubState = pCell->getState();
		if (aStatesToAvoid.contains(ubState)) return 0xFFFFu;

		if (aStatesPickups.contains(ubState)) return uiDistance;

		oNext = L::warpPoint(oNext, eDirection);

		// gone around field?
		if (oNext == oStart) return 0xFFFFu;

		uiDistance++;

	} // loop 0xFFFFu > uiDistance

	return uiDistance;

} // findNextGood


// virtual copy of worm.vala Worm.can_move_to(....)
bool SurfaceGame::aiCanMoveTo(Worm *pWorm) {

	if (pWorm->isImmune()) return true;

	SurfaceCell *pCell = this->getCell(pWorm->nextPoint());
	if (pCell->isNull()) return false;

	static QVector<quint8> aStatesToAvoid =
			IconEngine::statesSnakes() + IconEngine::statesWalls();

	return !aStatesToAvoid.contains(pCell->getState());

} // aiCanMoveTo


// virtual copy of worm.vala Worm::ai_deadend_after(....)
/* Check whether the worm will be trapped in a dead end. A location
 * within the dead end and the length of the worm is given. This
 * prevents worms getting trapped in a spiral, or in a corner sharper
 * than 90 degrees.  runnumber is a unique number used to update the
 * deadend board. The principle of the deadend board is that it marks
 * all squares previously checked, so the exact size of the deadend
 * can be calculated in O(n) time; to prevent the need to clear it
 * afterwards, a different number is stored in the board each time
 * (the number will not have been previously used, so the board will
 * appear empty). Although in theory deadend_runnumber may wrap round,
 * after 4 billion steps the entire board is likely to have been
 * overwritten anyway.
 */
qint32 SurfaceGame::aiDeadend(const QPoint oStart, qint32 ilLen) {

	static QVector<quint8> aStatesToAvoid =
			IconEngine::statesSnakes() + IconEngine::statesWalls();

	if (0 >= ilLen) return 0;

	QPoint oNext;

	for (quint8 ubDirection = L::North; ubDirection <= L::East; ubDirection++) {

		oNext = L::warpPoint(oStart, ubDirection);

		if ((!aStatesToAvoid.contains(this->getCell(oNext)->getState()))
				&& (this->ubAIcountDeadendRun != this->pAImap->tile(oNext))) {

			this->pAImap->setTile(oNext, this->ubAIcountDeadendRun);
			ilLen = this->aiDeadend(oNext, ilLen - 1);
			if (0 >= ilLen) return 0;

		} // if free cell

	} // loop all directions

	return ilLen;

} // aiDeadend


// virtual copy of worm.vala Worm::ai_deadend_after(....)
/* Check a deadend starting from the next square in this direction,
 * rather than from this square. Also block off the squares near worm
 * heads, so that humans can't kill AI players by trapping them
 * against a wall.  The given length is quartered and squared; this
 * allows for the situation where the worm has gone round in a square
 * and is about to get trapped in a spiral. However, it's set to at
 * least BOARDWIDTH, so that on the levels with long thin paths a worm
 * won't start down the path if it'll crash at the other end.
 */
qint32 SurfaceGame::aiDeadendAfter(Worm *pWorm, const qint32 ilLen) {

	if (this->getCell(pWorm->nextPoint())->isNull()) return 0;

	if (255u == this->ubAIcountDeadendRun) {

		// this should never happen the way I've set it up to clear
		// counter on each call to onMove (a bit much, only goes up to 23 with 8 worms)
		this->onDebugMessage("ran out of space in deadendnumber SurfaceGame::aiDeadendAfter");

		return 0;

	} // if ran out of space
	this->ubAIcountDeadendRun++;

	qint32 ilCl;
	QPoint oNext = pWorm->nextPoint();
	quint8 ubX = quint8(oNext.x());
	quint8 ubY = quint8(oNext.y());

	QPoint oHeadOther;
	L::Heading eDirection = pWorm->currentDirection();

	Worm *pWormOther;
	for (int i = 0; i < this->apWorms.length(); ++i) {

		pWormOther = this->apWorms.at(i);
		if (pWormOther == pWorm) continue; // ???
		if (pWormOther->isDead()) continue;

		oHeadOther = pWormOther->headCell()->getPos();

		if (oHeadOther.x() != ubX || oHeadOther.y() != ubY) {

			if (0 < iCx) this->pAImap->setTile(iCx - 1, iCy, this->uiAIdeadendRunnumber);
			if (0 < iCy) this->pAImap->setTile(iCx, iCy - 1, this->uiAIdeadendRunnumber);
			if ((SssS_Nibblers_Surface_Width - 1) > iCx) this->pAImap->setTile(iCx + 1, iCy, this->uiAIdeadendRunnumber);
			if ((SssS_Nibblers_Surface_Height -1) > iCy) this->pAImap->setTile(iCx, iCy + 1, this->uiAIdeadendRunnumber);

		} // if

	} // loop worms

	this->pAImap->setTile(oNext, this->ubAIcountDeadendRun);

	oNext = L::warpPoint(oNext, eDirection);

	this->pAImap->setTile(oNext, this->ubAIcountDeadendRun);

	ilCl = (ilLen * ilLen) / 16;
	if (SssS_Nibblers_Surface_Width > ilCl) ilCl = SssS_Nibblers_Surface_Width;

	return aiDeadend(oNext, ilCl);

} // aiDeadendAfter


// virtual copy of worm.vala Worm.ai_move(....)
void SurfaceGame::aiMove(Worm *pWorm) {

	//L::Heading eOpposite = L::oppositeHeading(pWorm->currentDirection());

	bool bAhead = aiWander(pWorm->nextPoint(), pWorm->nextPoint(), pWorm->currentDirection());
	bool bLeft = aiWander(pWorm->leftPoint(), pWorm->leftPoint(), pWorm->headingLeft());
	bool bRight = aiWander(pWorm->rightPoint(), pWorm->rightPoint(), pWorm->headingRight());

	if (!bAhead) {

		if (bLeft) pWorm->onTurnLeft();
		else if (bRight) pWorm->onTurnRight();
		else {

			// move randomly
			if ((qrand() % 31) == 1) {

				if (qrand() & 1) pWorm->onTurnLeft();
				else pWorm->onTurnRight();

			} // if random turn

		} // if left, right or ahead


	} // if not ahead

	// update eOpposite?

	/* Avoid walls, dead-ends and other worm's heads. This is done using
	 * an evalution function which is CAPACITY for a wall, 4 if another
	 * worm's head is in the tooclose area, 4 if another worm's head
	 * could move to the same location as ours, plus 0 if there's no
	 * dead-end, or the amount that doesn't fit for a deadend. olddir's
	 * score is reduced by 100, to favour it, but only if its score is 0
	 * otherwise; this is so that if we're currently trapped in a dead
	 * end, the worm will move in a space-filling manner in the hope
	 * that the dead end will disappear (e.g. if it's made from the tail
	 * of some worm, as often happens).
	 */
	L::Heading eDirectionOld = pWorm->currentDirection();
	static const quint16 uiCapacity = SssS_Nibblers_Surface_Height * SssS_Nibblers_Surface_Width;
	qint32 ilBestYet = uiCapacity * 2;
	quint8 ubDirectionBest = 0u;
	qint32 ilLen;

	for (quint8 ubDirection = 0; ubDirection < 3; ++ubDirection) {

		ilLen = 0;

		if (0u == ubDirection) {
			// ahead


		} else if (1u == ubDirection) {
			// left
			pWorm->onTurnLeft();

		} else {
			// right
			pWorm->onTurnRight();
			pWorm->onTurnRight();

		} // switch direction

		if (!this->aiCanMoveTo(pWorm)) ilLen += uiCapacity;

		if (this->aiTooClose(pWorm)) ilLen += 4;

		ilLen += aiDeadendAfter(pWorm, ilLen);

		// favour current direction
		if ((pWorm->currentDirection() == eDirectionOld) && (0 >= ilLen))
			ilLen -= 100;

		/* If the favoured direction isn't appropriate, then choose
		 * another direction at random rather than favouring one in
		 * particular, to stop the worms bunching in the bottom-
		 * right corner of the board.
		 */
		if (0 >= ilLen) {

			ilLen -= (qrand() % 101);

		} // if introduce noise

		if (iLen < iBestYet) {

			iBestYet = iLen;
			ibDirectionBest = pWorm->currentDirection();

		} // if better found

	} // loop 3 directions

	if (0 < ibDirectionBest) pWorm->setHeading(L::headingOfUint(ibDirectionBest));

	/* Make sure we are at least avoiding walls.
	 * Mostly other snakes should avoid our head.
	 */

	if (!this->aiCanMoveTo(pWorm)) {

		pWorm->onTurnLeft();

		if (!this->aiCanMoveTo(pWorm)) {

			// try right
			pWorm->onTurnRight();
			pWorm->onTurnRight();

		} // if can not go left

	} // if can not go ahead

} // aiMove


// virtual copy of worm.vala Worm.ai_too_close(....)
/* Check to see if another worm's head is too close in front of us;
 * that is, that it's within 3 in the direction we're going and within
 * 1 to the side.
 */
bool SurfaceGame::aiTooClose(Worm *pWorm) {

	if (pWorm->isImmune()) return false;

	int iDx, iDy;
	QPoint oHead = pWorm->headCell()->getPos();
	QPoint oHeadOther;
	L::Heading eDirection = pWorm->currentDirection();

	Worm *pWormOther;
	for (int i = 0; i < this->apWorms.length(); ++i) {

		pWormOther = this->apWorms.at(i);
		if (pWormOther == pWorm) continue;
		if (pWormOther->isDead()) continue;

		oHeadOther = pWormOther->headCell()->getPos();

		iDx = oHead.x() - oHeadOther.x();
		iDy = oHead.y() - oHeadOther.y();

		switch (eDirection) {

			case L::North:
				if ((iDy > 0) && (iDy <= 3) && (iDx >= -1) && (iDx <= 1))
					return true;
			break;

			case L::South:
				if ((iDy < 0) && (iDy >= -3) && (iDx >= -1) && (iDx <= 1))
					return true;
			break;

			case L::West:
				if ((iDx > 0) && (iDx <= 3) && (iDy >= -1) && (iDy <= 1))
					return true;
			break;

			case L::East:
				if ((iDx < 0) && (iDx >= -3) && (iDy >= -1) && (iDy <= 1))
					return true;
			break;
			default: break;

		} // switch eDirection

	} // loop worms

	return false;

} // aiTooClose


// virtual copy of worm.vala Worm::ai_wander(....)
bool SurfaceGame::aiWander(const QPoint oStart, const QPoint oStop,
						   const L::Heading eDirection) {

	static QVector<quint8> aStatesToAvoid =
			IconEngine::statesSnakes() + IconEngine::statesWalls();

	QPoint oNext = L::warpPoint(oStart, eDirection);

	SurfaceCell *pCell = this->getCell(oStart);
	if (pCell->isNull()) return false;

	quint8 ubState = pCell->getState();
	if (aStatesToAvoid.contains(ubState)) return false;

	switch (ubState) {

		case L::BonusAppleNW:
		case L::BonusAppleNE:
		case L::BonusAppleSE:
		case L::BonusAppleSW:
			return true;

		case L::BonusCherryNW:
		case L::BonusCherryNE:
		case L::BonusCherrySE:
		case L::BonusCherrySW:
			return false;

		case L::BonusBananaNW:
		case L::BonusBananaNE:
		case L::BonusBananaSE:
		case L::BonusBananaSW:
			return true;

		case L::BonusHeartNW:
		case L::BonusHeartNE:
		case L::BonusHeartSE:
		case L::BonusHeartSW:
			return true;

		case L::BonusDiamondNW:
		case L::BonusDiamondNE:
		case L::BonusDiamondSE:
		case L::BonusDiamondSW:
			return true;

		default:

			if (oNext == oStop) return false;

			return aiWander(oNext, oStop, eDirection);

		break;

	} // switch this->getCell(oStart)->getState()

} // aiWander


void SurfaceGame::findNextMovesForWorm(Worm *pWorm) {

	static QVector<quint8> aStatesPickups = IconEngine::statesPickups();
	//static QVector<quint8> aStatesTeleporterEntrances = IconEngine::statesTeleporterEntrances();
	//static QVector<quint8> aStatesTeleporterExits = IconEngine::statesTeleporterExits();
	static QVector<quint8> aStatesToAvoid =
			IconEngine::statesSnakes() + IconEngine::statesWalls();

	SurfaceCell *pCell = this->getCell(pWorm->nextPoint());
	if (pCell->isNull()) return;

//	bool bCanGoAhead = true;
	bool bCanTurnLeft = false;
	bool bCanTurnRight = false;
	quint8 ubCanTurnCount;

	quint8 ubState = pCell->getState();

	if (aStatesToAvoid.contains(ubState)) {

//		bCanGoAhead = false;

		// need to change direction if possible

		bCanTurnLeft = !aStatesToAvoid.contains(
						   this->getCell(pWorm->leftPoint())->getState());

		bCanTurnRight = !aStatesToAvoid.contains(
							this->getCell(pWorm->rightPoint())->getState());

		ubCanTurnCount = quint8(bCanTurnLeft) + quint8(bCanTurnRight);

		// no safe direction
		if (0u == ubCanTurnCount) return;

		// either direction is free
		if (2u == ubCanTurnCount) {

			// pick a direction
			quint16 uiLeftGoodIn = this->findNextGood(pWorm->leftPoint(), pWorm->headingLeft());
			quint16 uiRightGoodIn = this->findNextGood(pWorm->rightPoint(), pWorm->headingRight());
			if (0u == uiLeftGoodIn + uiRightGoodIn) {
				// both have bonus
				if ((qrand() & 1) == 1) pWorm->onTurnLeft();
				else pWorm->onTurnRight();

			}

			return;

		} // if can go either way

		// can only go one way
		if (bCanTurnLeft) pWorm->onTurnLeft();
		else pWorm->onTurnRight();

		return;

	} else if (aStatesPickups.contains(ubState)) {

		// we're good, keep going
		return;

	} // if need to imediately turn or keep going

	// cell ahead contains no imediate danger or bonus
	// find the nearest bonus (and prioratize)

	pCell = pWorm->headCell();
	if (pCell->isNull()) return;

	QVector<quint8> aubAhead = this->nextPOIinDirection(pCell, pWorm->currentDirection());
	QVector<quint8> aubLeft = this->nextPOIinDirection(pCell, pWorm->headingLeft());
	QVector<quint8> aubRight = this->nextPOIinDirection(pCell, pWorm->headingRight());

	// which is the most promising?
	QVector<quint8> aubGoodDistances;
	if (2u == aubAhead.at(2)) aubGoodDistances.append(1u);
	if (2u == aubLeft.at(2)) aubGoodDistances.append(2u);
	if (2u == aubRight.at(2)) aubGoodDistances.append(3u);

	// which is nearest?
	quint8 ubNearestDistance = 0xFFu;
	quint8 ubIndexNearestDistance = 0u;
	if (aubGoodDistances.contains(1u) && ubNearestDistance > aubAhead.at(1)) {
		ubNearestDistance = aubAhead.at(1);
		ubIndexNearestDistance = 1u;
	} // if ahead has something good

	if (aubGoodDistances.contains(2u) && ubNearestDistance > aubLeft.at(1)) {
		ubNearestDistance = aubLeft.at(1);
		ubIndexNearestDistance = 2u;
	} // if left has something good

	if (aubGoodDistances.contains(3u) && ubNearestDistance > aubRight.at(1)) {
		ubNearestDistance = aubRight.at(1);
		ubIndexNearestDistance = 3u;
	} // if right has something good

	switch (ubIndexNearestDistance) {

		case 3u: pWorm->onTurnRight(); return;
		case 2u: pWorm->onTurnLeft(); return;
		case 1u: return;

		case 0u:
		default:
		break;

	} // switch ubIndexNearestDistance

	// nothing promising to go for
	// randomly change direction
	if (!((qrand() & 1u) && (qrand() & 1u))) return;

	// which is the most promising?
	QVector<quint8> aubBadDistances;
	if ((1u == aubAhead.at(2)) && (0u < aubAhead.at(1))) aubBadDistances.append(1u);
	if ((1u == aubLeft.at(2)) && (0u < aubLeft.at(1))) aubBadDistances.append(2u);
	if ((1u == aubRight.at(2)) && (0u < aubRight.at(1))) aubBadDistances.append(3u);

	// which is farthest?
	quint8 ubFarthestDistance = 0u;
	quint8 ubIndexFarthestDistance = 0u;
	if (aubBadDistances.contains(1u) && (ubFarthestDistance < aubAhead.at(1))) {
		ubFarthestDistance = aubAhead.at(1);
		ubIndexFarthestDistance = 1u;
	} // if ahead has something bad

	if (aubBadDistances.contains(2u) && (ubFarthestDistance < aubLeft.at(1))) {
		ubFarthestDistance = aubLeft.at(1);
		ubIndexFarthestDistance = 2u;
	} // if left has something bad

	if (aubBadDistances.contains(3u) && (ubFarthestDistance < aubRight.at(1))) {
		ubFarthestDistance = aubRight.at(1);
		ubIndexFarthestDistance = 3u;
	} // if right has something bad

	switch (ubIndexFarthestDistance) {

		case 3u: pWorm->onTurnRight(); return;
		case 2u: pWorm->onTurnLeft(); return;
		case 1u: return;

		case 0u:
			// should not happen
		default:
		break;

	} // switch ubIndex

} // findNextMovesForWorm


SurfaceCell *SurfaceGame::getCell(const QPoint oPoint) {

	return this->getCell(oPoint.x(), oPoint.y());

} // getCell(point)


SurfaceCell *SurfaceGame::getCell(const quint8 ubColumn, const quint8 ubRow) {

	// check limits
	if (ubRow >= this->aopRows.count()) return new SurfaceCell();
	if (ubColumn >= this->aopRows.first().count()) return new SurfaceCell();

	QList<SurfaceCell *> aRow = this->aopRows.at(ubRow);
	SurfaceCell *pCell = aRow.at(ubColumn);

	return pCell;

} // getCell(x,y)


void SurfaceGame::init() {

	this->initCells();

	this->loadCurrentLevel();

	this->initWorms();

	this->initKeys();

	this->resetButtons();

} // init


void SurfaceGame::initCells() {

	quint8 ubColumns = 0u;
	quint8 ubRows = 0u;
	quint8 ubState = L::FloorClean;
	QList<SurfaceCell*> aRow;
	SurfaceCell *pCell;
	QHBoxLayout *pHBox;
	QVBoxLayout *pVBox = new QVBoxLayout();

	pVBox->setSpacing(0);

	for (; ubRows < SssS_Nibblers_Surface_Height; ++ubRows) {

		aRow.clear();
		pHBox = new QHBoxLayout();

		for (ubColumns = 0u; ubColumns < SssS_Nibblers_Surface_Width; ++ubColumns) {

			pCell = new SurfaceCell(false, ubState, ubColumns, ubRows, this);

			aRow.append(pCell);
			pHBox->addWidget(pCell);

			connect(pCell, SIGNAL(debugMessage(QString)),
					this, SLOT(onDebugMessage(QString)));

		} // loop columns

		this->aopRows.append(aRow);
		pVBox->addItem(pHBox);

	} // loop rows

	this->pUi->frameSurface->setLayout(pVBox);

} // initCells


void SurfaceGame::initKeys() {

	this->ahKeys.clear();

	QHash<QKeySequence, L::Heading> hKeys;
	QKeySequence oKS;
	quint8 ubTotalHumans = this->pAS->get(AppSettings::sSettingGameCountHumans).toUInt();

	for (quint8 ubWorm = 0u; ubWorm < ubTotalHumans; ++ubWorm) {

		hKeys.clear();
		oKS = QKeySequence(this->pAS->getPlayerKeyDown(ubWorm),
						   QKeySequence::PortableText);
		hKeys.insert(oKS, L::Down);
		oKS = QKeySequence(this->pAS->getPlayerKeyLeft(ubWorm),
						   QKeySequence::PortableText);
		hKeys.insert(oKS, L::Left);
		oKS = QKeySequence(this->pAS->getPlayerKeyRight(ubWorm),
						   QKeySequence::PortableText);
		hKeys.insert(oKS, L::Right);
		oKS = QKeySequence(this->pAS->getPlayerKeyUp(ubWorm),
						   QKeySequence::PortableText);
		hKeys.insert(oKS, L::Up);

		this->ahKeys.append(hKeys);

	} // loop

} // initKeys


void SurfaceGame::initWorms() {

	this->apWorms.clear();
	this->clearSurfaceOfWorms();

	quint8 ubColour;
	quint8 ubCount;
	quint8 ubCountAIs = this->pAS->get(AppSettings::sSettingGameCountAIs).toUInt();
	quint8 ubCountHumans = this->pAS->get(AppSettings::sSettingGameCountHumans).toUInt();
	quint8 ubLives = this->pAS->get(AppSettings::sSettingGameStartLives).toUInt();

	QVector<SurfaceCell *> aUsedSpawns;
	SurfaceCell *pCell;
	Worm *pWorm;
	ScoreBoard *pSB;
	quint8 ubCountAll = ubCountAIs + ubCountHumans;

	// check that there are enough spawn points
	if (ubCountAll > this->apSpawnPoints.length()) {

		this->pUi->buttonPP->setEnabled(false);
		Q_EMIT this->statusMessage(tr("Level does not have sufficient spawn-points. Bailling."));
		return;

	} // if not enough start points

	for (ubCount = 0u; ubCount < ubCountAll; ++ubCount) {

		// TODO: random spawn point distribution
		pCell = this->apSpawnPoints.at(ubCount);

		ubColour = this->pAS->getPlayerColour(ubCount);

		pWorm = new Worm(pCell, ubColour, (ubCount >= ubCountHumans), this);

		pWorm->setUseRelativeControls(this->pAS->getPlayerRelative(ubCount));

		connect(pWorm, SIGNAL(debugMessage(QString)),
				this, SLOT(onDebugMessage(QString)));

		pSB = new ScoreBoard(ubColour, this);
		this->pUi->frameScore->layout()->addWidget(pSB);

		connect(pSB, SIGNAL(debugMessage(QString)),
				this, SLOT(onDebugMessage(QString)));

		// connect worm with score board
		connect(pWorm, SIGNAL(updateColour(quint8)),
				pSB, SLOT(setColour(quint8)));

		connect(pWorm, SIGNAL(updateLives(quint8)),
				pSB, SLOT(setLives(quint8)));

		connect(pWorm, SIGNAL(updateName(QString)),
				pSB, SLOT(setName(QString)));

		connect(pWorm, SIGNAL(updateScore(quint32)),
				pSB, SLOT(setScore(quint32)));

		aUsedSpawns.append(pCell);
		this->apWorms.append(pWorm);
		this->apScoreBoards.append(pSB);

		Q_EMIT this->wormCreated(pWorm);

		pWorm->onSetLives(ubLives);
		pWorm->onSetName(tr("Player ") + QString::number(ubCount + 1u));

	} // loop worms

} // initWorms


void SurfaceGame::keyPressEvent(QKeyEvent *pEvent) {

	QKeySequence oKSin(pEvent->key());
	QKeySequence oKey2(" ");

	if (oKSin == oKey2) this->onDebugMessage("Matched K");
//	else
	this->onDebugMessage(QString::number(pEvent->key()));

	quint8 ubWorm;
	QHash<QKeySequence, L::Heading> hKeys;
	bool bNoMatchFound = true;

	for (ubWorm = 0u; ubWorm < this->ahKeys.length(); ++ubWorm) {

		hKeys = this->ahKeys.at(ubWorm);
		if (!hKeys.contains(oKSin)) continue;

		bNoMatchFound = false;

		this->apWorms.at(ubWorm)->onTurn(hKeys.value(oKSin));

	} // loop each set of keys

	if (bNoMatchFound) QFrame::keyPressEvent(pEvent);

} // keyPressEvent


void SurfaceGame::loadCurrentLevel() {

	QString sPath = this->pAS->getDataPathLevelFile(this->ubCurrentLevel);

	this->pUi->labelLevel->setText(tr("Level ") + QString::number(this->ubCurrentLevel));

	this->clearSurface();

	QFileInfo oFI = QFileInfo(sPath);
	if (!(oFI.exists() && oFI.isFile())) {

		this->onDebugMessage("KO: failed to find: " + sPath);
		return;

	} // if file does not exist

	QFile oFile(sPath);
	if (!oFile.open(QFile::ReadOnly)) {

		this->onDebugMessage("KO: failed to open: " + sPath);
		return;

	} // if failed to open

	QByteArray aFile = oFile.readAll();
	oFile.close();

	if ((SssS_Nibblers_Surface_Height * SssS_Nibblers_Surface_Width)
			> aFile.length()) {

		this->onDebugMessage("invalid length (too short)");
		return;

	} // if invalid length

	static QVector<quint8> aStatesSpawns = IconEngine::statesSpawns();
	static QVector<quint8> aStatesTeleporterEntrances = IconEngine::statesTeleporterEntrances();
	static QVector<quint8> aStatesTeleporterExits = IconEngine::statesTeleporterExits();

	int iPos = 0u;
	quint8 ubColumns = 0u;
	quint8 ubRows = 0u;
	QList<SurfaceCell*> aRow;
	SurfaceCell *pCell;
	quint8 ubState = L::FloorClean;

	for (; ubRows < SssS_Nibblers_Surface_Height; ++ubRows) {

		aRow = this->aopRows.at(ubRows);

		for (ubColumns = 0u; ubColumns < SssS_Nibblers_Surface_Width; ++ubColumns) {

			pCell = aRow.at(ubColumns);

			ubState = quint8(aFile.at(iPos));

			// keep track of special ones

			if (aStatesSpawns.contains(ubState)) {

				this->apSpawnPoints.append(pCell);

			} else if (aStatesTeleporterEntrances.contains(ubState)) {

				this->hpTeleporterEntrances.insert(ubState, pCell);

			} else if (aStatesTeleporterExits.contains(ubState)) {

				this->hpTeleporterExits.insert(ubState, pCell);

			} // if special state we need to keep track of (new state)

			this->setCellState(pCell, ubState);
			pCell->freezeState();

			iPos++;

		} // loop columns

	} // loop rows

} // loadCurrentLevel


// old method that wasn't quite completed
QVector<quint8> SurfaceGame::nextPOIinDirection(SurfaceCell *pCell,
											 const L::Heading eDirection) {

	static QVector<quint8> aStatesPickups = IconEngine::statesPickups();
	//static QVector<quint8> aStatesTeleporterEntrances = IconEngine::statesTeleporterEntrances();
	//static QVector<quint8> aStatesTeleporterExits = IconEngine::statesTeleporterExits();
	static QVector<quint8> aStatesToAvoid =
			IconEngine::statesSnakes() + IconEngine::statesWalls();

	QVector<quint8> aubResult;
	quint8 ubX = pCell->getColumn();
	quint8 ubY = pCell->getRow();
	int iDx = 0;
	int iDy = 0;

	switch (eDirection) {

		case L::North: iDy = -1; break;
		case L::South: iDy = 1; break;
		case L::West: iDx = -1; break;
		case L::East: iDx = 1; break;

		default: break;

	} // switch eDirection

	quint8 ubDistance = 0;
	quint8 ubState;
	while (true) {

		ubDistance++;

		if (0 > iDx) {
			if (0 == ubX) ubX = SssS_Nibblers_Surface_Width - 1u;
			else ubX--;
		} else {
			ubX++;
			if (SssS_Nibblers_Surface_Width <= ubX) ubX = 0u;
		} // if horizontal direction

		if (0 > iDy) {
			if (0 == ubY) ubY = SssS_Nibblers_Surface_Height - 1u;
			else ubY--;
		} else {
			ubY++;
			if (SssS_Nibblers_Surface_Height <= ubY) ubY = 0u;
		} // if vertical direction

		ubState = this->getCell(ubX, ubY)->getState();

		if (aStatesToAvoid.contains(ubState)) {

			aubResult.append(ubState);
			aubResult.append(ubDistance);
			aubResult.append(1u);
			return aubResult;

		} // if something to avoid encountered

		if (aStatesPickups.contains(ubState)) {

			aubResult.append(ubState);
			aubResult.append(ubDistance);
			aubResult.append(2u);
			return aubResult;

		} // if something worth while encountered

	} // loop true

	// should never happen but jic
	aubResult.append(L::NullCell);
	aubResult.append(0xFFu);
	aubResult.append(0xFFu);
	return aubResult;

} // nextPOIinDirection


void SurfaceGame::on_buttonPP_toggled(bool bChecked) {

	this->pUi->buttonPP->setText(bChecked ? tr("Pause") : tr("Play"));

	if (this->bProtectPP) return;

	if (this->pStartCountDownFrame) {
	if (!bChecked) {
		this->pStartCountDownFrame->onSetText(tr("Paused"));
		this->pStartCountDownFrame->setGeometry(this->pUi->frameSurface->geometry());
		this->pStartCountDownFrame->move(mapToGlobal(QPoint(this->pUi->frameSurface->geometry().left(), this->pUi->frameSurface->geometry().top())));//, this->pUi->buttonPP->height() + 12);
		this->pStartCountDownFrame->show();
		} else this->pStartCountDownFrame->hide();
	}

	Q_EMIT this->pauseResumeToggled();

	if (bChecked) this->setFocus();

} // on_buttonPP_toggled


void SurfaceGame::on_buttonSR_clicked() {

	// pause game if running
	this->pauseIfRunning();

	// open restart dialog
	if (nullptr == this->pDialogLoad) {

		this->pDialogLoad = new DialogLoad(this);

		connect(this->pDialogLoad, SIGNAL(finished(int)),
			this, SLOT(dialogLoadFinished(int)));

	} // if first time

	this->pDialogLoad->setSelected(this->ubCurrentLevel);

	this->pDialogLoad->exec();

} // on_buttonSR_clicked


void SurfaceGame::onColoursChanged(const QVector<quint8> aubColours) {

	// better safe than sorry
	if (aubColours.length() < this->apWorms.length()) return;

	quint8 ubWorm;
	for (ubWorm = 0u; ubWorm < this->apWorms.length(); ++ubWorm) {

		this->apWorms.at(ubWorm)->setColourIndex(aubColours.at(ubWorm));

	} // loop

} // onColoursChanged


void SurfaceGame::onDoGameOver() {

	// button is probably showing "Pause"
	if (this->pUi->buttonPP->isChecked()) {
		this->bProtectPP = true;
		this->pUi->buttonPP->setChecked(false);
		this->pUi->buttonPP->setEnabled(false);
		this->bProtectPP = false;
	}

	// show game over dialog

	Q_EMIT this->statusMessage("Game Over");

} // onDoGameOver


void SurfaceGame::onDoLevelStartCountdown() {

	// go 5 steps in original direction
	this->onMove();
	this->onMove();
	this->onMove();
	this->onMove();
	this->onMove();

	// open count-down dialog
	if (nullptr == this->pStartCountDownFrame) {

		FrameStartCountdown *pF = new FrameStartCountdown();
		this->pStartCountDownFrame = pF;
		pF->setWindowFlags(Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);

	} // if first time

	this->pStartCountDownFrame->onSetText("3");
	this->pStartCountDownFrame->setGeometry(this->window()->geometry());
	this->pStartCountDownFrame->show();

	this->pUi->buttonPP->setText("3");
	this->pUi->buttonPP->setEnabled(false);
	this->pUi->buttonSR->setEnabled(false);

	Q_EMIT this->statusMessage(tr("Get Ready: 3..."));

	QTimer::singleShot(1000, this, SLOT(countdownTick()));

} // onDoLevelStartCountdown


void SurfaceGame::onMainTabChanged(const int iIndex) {
	Q_UNUSED(iIndex)

	//this->pauseIfRunning();

} // onMainTabChanged


void SurfaceGame::onMove() {

	static QVector<quint8> aStatesPickups = IconEngine::statesPickups();
	static QVector<quint8> aStatesSnakes = IconEngine::statesSnakes();
	static QVector<quint8> aStatesSpawns = IconEngine::statesSpawns();
	static QVector<quint8> aStatesTeleporterEntrances = IconEngine::statesTeleporterEntrances();
	static QVector<quint8> aStatesTeleporterExits = IconEngine::statesTeleporterExits();
	static QVector<quint8> aStatesWalls = IconEngine::statesWalls();

	Worm *pWorm;
	SurfaceCell *pCell;
	quint8 ubState;
	// keep track of cells that are being newly occupied
	QHash<SurfaceCell *, Worm *> hppCrashPotential;
	// keep track of worms that are crashing
	QVector<Worm *> apCrashedWorms;
	hppCrashPotential.clear();
	apCrashedWorms.clear();

	// collect immediate threats and goodies
	for (int i = 0; i < this->apWorms.length(); ++i) {

		pWorm = this->apWorms.at(i);

		if (pWorm->isDead()) continue;

		pCell = this->getCell(pWorm->nextPoint());
		if (pCell->isNull()) continue;

		if (pWorm->isImmune()) {

			addCrashPotential(hppCrashPotential, apCrashedWorms, pCell, pWorm);
			pWorm->advanceTo(pCell);
			continue;

		} //

		ubState = pCell->getState();

		if (aStatesSnakes.contains(ubState)
				|| aStatesWalls.contains(ubState)) {

			// crash
			apCrashedWorms.append(pWorm);
			continue;

		} else if (aStatesTeleporterEntrances.contains(ubState)) {

			// enter teleporter
			if (this->hpTeleporterExits.contains(ubState + 1u)) {

				// add both entrance and exit to crash potentials
				addCrashPotential(hppCrashPotential, apCrashedWorms,
								  pCell, pWorm);
				addCrashPotential(hppCrashPotential, apCrashedWorms,
								  this->hpTeleporterExits.value(ubState + 1u),
								  pWorm);

				// advance to both locations
				pWorm->advanceTo(pCell);
				pWorm->advanceTo(this->hpTeleporterExits.value(ubState + 1u));
				continue;

			} // if matching exit exists

		} // if not empty

		if (aStatesPickups.contains(ubState)) {

			// let Game figure out which bonus it is
			Q_EMIT this->wormAteBonus(pWorm, pCell);

			// mark bloatedness on worm
			pCell->addBloatedHeading(pWorm->currentDirection());
			pWorm->advanceTo(pCell);
			//pWorm->setNextBloatHeading(L::oppositeHeading(pWorm->currentDirection()));
		} // if picked up something

		pWorm->advanceTo(pCell);

		SurfaceGame::addCrashPotential(hppCrashPotential, apCrashedWorms,
									   pCell, pWorm);

	} // loop

	// deal with crashed worms
	for (int i = 0; i < apCrashedWorms.length(); ++i) {

		pWorm = apCrashedWorms.at(i);

		this->clearSurfaceOfWorm(pWorm->colourIndex());

		Q_EMIT this->wormCrashed(pWorm);

	} // loop crashed worms

	// do AI-moves
	for (int i = 0; i < this->apWorms.length(); ++i) {

		pWorm = this->apWorms.at(i);

		if (!pWorm->isAI()) continue;

		if (pWorm->isDead()) continue;

		this->aiMove(pWorm);
		//this->findNextMovesForWorm(pWorm);

	} // loop worms

} // onMove


void SurfaceGame::onNextLevel() {

	if (0xFFu == this->ubCurrentLevel) this->ubCurrentLevel = 0u;
	else this->ubCurrentLevel++;

	this->loadCurrentLevel();

	// distribute spawn points. this could be done by Game

} // onNextLevel


void SurfaceGame::onPlaceBonus(const quint8 ubBonus, const bool bFake) {

	// find 4 adjacent cells that are not occupied

	static QVector<quint8> aubStatesFree = IconEngine::statesFloors()
										 + IconEngine::statesTeleporterExits()
										 + IconEngine::statesSpawns();



	quint8 ubColumns = 0u;
	quint8 ubRows = 0u;
	SurfaceCell *pCell;
	SurfaceCell *pCell1;
	SurfaceCell *pCell2;
	SurfaceCell *pCell3;
	QVector<SurfaceCell *> apFreeCells;
	apFreeCells.clear();

	for (; ubRows < SssS_Nibblers_Surface_Height; ++ubRows) {

		for (ubColumns = 0u; ubColumns < SssS_Nibblers_Surface_Width; ++ubColumns) {

			pCell = this->getCell(ubColumns, ubRows);
			if (!aubStatesFree.contains(pCell->getState())) continue;

			pCell1 = this->getCell(L::warpPoint(ubColumns, ubRows, L::Right));
			if (!aubStatesFree.contains(pCell1->getState())) continue; // TODO: optimize as we have already checked this one

			pCell2 = this->getCell(L::warpPoint(ubColumns, ubRows, L::Down));
			if (!aubStatesFree.contains(pCell2->getState())) continue;

			pCell3 = this->getCell(L::warpPoint(pCell2->getColumn(), pCell2->getRow(), L::Right));
			if (!aubStatesFree.contains(pCell3->getState())) continue;

			// OK, this one could work
			apFreeCells.append(pCell);

		} // loop columns

	} // loop rows

	int iMaxPlusOne = apFreeCells.length();
	if (0 == iMaxPlusOne) {

		Q_EMIT this->noSpaceFoundForBonus(ubBonus, bFake);

		return;

	} // if no free space to put any bonus

	int iMin = 0;
	int iIndex = iMin + (qrand() % (iMaxPlusOne - iMin));

	pCell = apFreeCells.at(iIndex);
	pCell1 = this->getCell(L::warpPoint(pCell->getColumn(), pCell->getRow(), L::Right));
	pCell2 = this->getCell(L::warpPoint(pCell->getColumn(), pCell->getRow(), L::Down));
	pCell3 = this->getCell(L::warpPoint(pCell2->getColumn(), pCell2->getRow(), L::Right));

	pCell->setState(ubBonus); pCell->update();
	pCell1->setState(ubBonus + 1u); pCell1->update();
	pCell2->setState(ubBonus + 3u); pCell2->update();
	pCell3->setState(ubBonus + 2u); pCell3->update();

	QVector<SurfaceCell *> apCells;
	apCells.clear();
	apCells.append(pCell);
	apCells.append(pCell1);
	apCells.append(pCell3);
	apCells.append(pCell2);

	Q_EMIT this->bonusPlaced(apCells, bFake);

} // onPlaceBonus


void SurfaceGame::onPlayerCountChanged(const quint8 ubCountHumans,
									   const quint8 ubCountAIs) {
	Q_UNUSED(ubCountAIs)
	Q_UNUSED(ubCountHumans)

	// TODO:

} // onPlayerCountChanged


void SurfaceGame::onPlayerKeyChanged(const quint8 ubWorm,
										   const QKeySequence &oKeySequence,
										   const L::Heading eHeading) {
	Q_UNUSED(ubWorm)
	Q_UNUSED(oKeySequence)
	Q_UNUSED(eHeading)

	this->initKeys();

} // onPlayerKeyChanged


void SurfaceGame::onPlayerRelativeChanged(const quint8 ubWorm,
										  const bool bRelative) {

	if (this->apWorms.length() <= ubWorm) return;

	this->apWorms.at(ubWorm)->setUseRelativeControls(bRelative);

} // onPlayerRelativeChanged


void SurfaceGame::onQuitting() {

	if (this->pStartCountDownFrame) this->pStartCountDownFrame->close();

} // onQuitting


void SurfaceGame::onSpawnWorm(Worm *pWorm) {

	pWorm->startSpawning();

} // onSpawnWorm


void SurfaceGame::onSpawnWorms() {

	this->onDebugMessage("spawn worssssssssssss");

	quint8 ubTotal = this->apWorms.length();
	quint8 ubCount = 0u;
	for (; ubCount < ubTotal; ++ubCount) {

//		this->onSpawnWorm(ubCount);

	} // loop worms

} // onSpawnWorms


void SurfaceGame::pauseIfRunning() {

	// pause game if running
	if (this->pUi->buttonPP->isChecked())
		this->pUi->buttonPP->setChecked(false);

} // pauseIfRunning


void SurfaceGame::resetButtons() {

	this->bProtectPP = true;
	this->pUi->buttonPP->setChecked(false);
	this->pUi->buttonPP->setEnabled(true);
	this->pUi->buttonSR->setEnabled(true);
	this->pUi->buttonPP->setText(tr("Start"));
	this->bProtectPP = false;

} // resetButtons


void SurfaceGame::resizeEvent(QResizeEvent *pEvent) {

	QFrame::resizeEvent(pEvent);

	int iSBwidth = 200;
	int iDiff = this->window()->width() - this->width();

	for (int i = 0; i < this->apScoreBoards.length(); ++i) {

		if (iDiff >= 2 * iSBwidth) {
			// place score-boards left and right of field
			if (i & 1) this->pUi->frameScoreRight->layout()->addWidget(this->apScoreBoards.at(i));
			else this->pUi->frameScoreLeft->layout()->addWidget(this->apScoreBoards.at(i));
		} else if (iDiff >= iSBwidth) {
			// place score-boards on left of field
			this->pUi->frameScoreLeft->layout()->addWidget(this->apScoreBoards.at(i));
		} else {
			// move them to bottom
			this->pUi->frameScore->layout()->addWidget(this->apScoreBoards.at(i));
		} // switch spare space

	} // loop

} // resizeEvent


void SurfaceGame::setCellState(SurfaceCell *pCell, const quint8 ubState,
								  const bool bUpdate) {

	quint8 ubStateOld = pCell->getState();

	// nothing to do?
	if (ubStateOld == ubState) return;

	// finally change the cell's state and update if requested

	pCell->setState(ubState);
	if (bUpdate) pCell->update();

} // setCellState


void SurfaceGame::setCellState(const quint8 ubColumn, const quint8 ubRow,
								  const quint8 ubState, const bool bUpdate) {

	// check limits
	if (ubRow >= this->aopRows.count()) return;
	if (ubColumn >= this->aopRows.first().count()) return;

	QList<SurfaceCell *> aRow = this->aopRows.at(ubRow);
	SurfaceCell *pCell = aRow.at(ubColumn);

	this->setCellState(pCell, ubState, bUpdate);

} // setCellState


QSize SurfaceGame::sizeHint() const {

	return QFrame::sizeHint();

} // sizeHint



}	} // namespace SwissalpS::QtNibblers

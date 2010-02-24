#ifndef BATTLEWINDOW_H
#define BATTLEWINDOW_H

#include <QtGui>
#include "../PokemonInfo/battlestructs.h"
#include "basebattlewindow.h"

class AttackZone;
class PokeZone;
class BattleDisplay;
class QScrollDownTextEdit;

struct BattleInfo : public BaseBattleInfo
{
    /* Possible choices */
    bool possible;
    BattleChoices choices;

    /* My team */
    TeamBattle myteam;
    const PokeBattle &currentPoke() const;
    PokeBattle &currentPoke();

    BattleStats mystats;

    /* Current poke for ourself */
    int currentIndex;
    int lastIndex;
    int validIndex() const {
        return currentIndex == -1 ? lastIndex : currentIndex;
    }
};

/* The battle window called by the client, online */

class Client;

class BattleWindow : public QWidget
{
    Q_OBJECT

    PROPERTY(BattleInfo, info);
    PROPERTY(BattleConfiguration, conf);
    PROPERTY(int, idme);
    PROPERTY(int, idopp);
    PROPERTY(Client *, client);
public:
    BattleWindow(const QString &me, const QString &opponent, int idme, int idopp, const TeamBattle &myteam, const BattleConfiguration &conf);

    TeamBattle &team();
    const TeamBattle &team() const;

    enum BattleCommand
    {
	SendOut,
	SendBack,
	UseAttack,
	OfferChoice,
	BeginTurn,
	ChangePP,
	ChangeHp,
	Ko,
	Effective, /* to tell how a move is effective */
	Miss,
	CriticalHit,
	Hit, /* for moves like fury double kick etc. */
	StatChange,
	StatusChange,
	StatusMessage,
	Failed,
	BattleChat,
	MoveMessage,
	ItemMessage,
	NoOpponent,
	Flinch,
	Recoil,
	WeatherMessage,
        StraightDamage,
        AbilityMessage,
        AbsStatusChange,
        Substitute,
        BattleEnd,
        BlankMessage,
        CancelMove,
        Clause,
        DynamicInfo,
        DynamicStats,
        Spectating,
        SpectatorChat
    };

    enum WeatherM
    {
	StartWeather,
	ContinueWeather,
	EndWeather,
	HurtWeather
    };

    enum Weather
    {
	NormalWeather = 0,
	Hail = 1,
	Rain = 2,
	SandStorm = 3,
	Sunny = 4
    };

    enum StatusFeeling
    {
	FeelConfusion,
	HurtConfusion,
	FreeConfusion,
	PrevParalysed,
	PrevFrozen,
	FreeFrozen,
	FeelAsleep,
	FreeAsleep,
	HurtBurn,
	HurtPoison
    };

    enum
    {
	ZoneOfPokes = 6,
	ZoneOfNothing = 7
    };

    void switchToNaught(bool self);
    void switchTo(int pokezone);

    /* Disable / enable buttons */
    void updateChoices();
    /* sends the choice */
    void sendChoice(const BattleChoice &b);

    void printLine(const QString &str);
    void printHtml(const QString &str);
    QString name(bool self) const;
    QString nick(bool self) const;
    QString rnick(bool self) const;

public slots:
    void receiveInfo(QByteArray);
    void switchClicked(int zone);
    void attackClicked(int zone);
    void sendMessage();
    void attackButton();
    void clickforfeit();
    void emitCancel();
    void switchToPokeZone();
signals:
    void battleCommand(const BattleChoice &);
    void battleMessage(const QString &str);
    void forfeit();
protected:
    void closeEvent(QCloseEvent *);
private:
    QStackedWidget *mystack;
    AttackZone *myazones[6];
    PokeZone *mypzone;
    QScrollDownTextEdit *mychat;
    QLineEdit *myline;
    BattleDisplay *mydisplay;
    QPushButton *myswitch, *myattack, *myforfeit, *mysend, *mycancel;

    bool blankMessage;
    bool battleEnded;

    /* What can I do? */

};

class GraphicsZone;

class BattleDisplay : public QWidget
{
    Q_OBJECT
public:
    const BattleInfo & info;

    BattleDisplay(const BattleInfo &i);

    void updatePoke(bool self);
    void updateToolTip(bool self);
    void changeStatus(bool self, int poke, int status);

protected:
    const PokeBattle &mypoke() const {return info.currentPoke(); }
    const ShallowBattlePoke &foe() const {return info.pokes[Opponent]; }

    QString health(int lifePercent);

    GraphicsZone *zone;
    QLabel *nick[2];
    QLabel *status[2];
    QProgressBar *bars[2];
    QLabel *gender[2];
    /* The pokeballs to indicate how well a team is doing */
    QLabel *advpokeballs[6];
    QLabel *mypokeballs[6];
};

/* The graphics zone, where both pokes are displayed */
class GraphicsZone : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsZone();
    /* displays that poke */
    template <class T>
    void switchTo(const T &poke, bool self, bool sub);
    /* Display blank */
    void switchToNaught(bool self);
    /* For tool tips */
    bool event(QEvent *event);

    /* Loads a pixmap if not loaded otherwise go see graphics */
    QPixmap loadPixmap(quint16 num, bool shiny, bool back, quint8 gender, bool sub);
    /* We are using a qmap to store the graphics already loaded. So the key of the pixmap
	is a combination of 2 bools, 1 quin8; and one quint16 */
    qint32 key(quint16 num, bool shiny, bool back, quint8 gender, bool sub) const;
    QHash<qint32, QPixmap> graphics;
    /* Current pixmaps displayed */
    QGraphicsPixmapItem *mine, *foe;
    QGraphicsScene scene;

    QString tooltips[2];
};


class AttackButton;

/* An attack zone is the zone where the attacks are displayed */
class AttackZone : public QWidget
{
    Q_OBJECT
public:
    AttackZone(const PokeBattle &poke);

    AttackButton *attacks[4];
signals:
    void clicked(int attack);

private:
    QSignalMapper *mymapper;
};

class AttackButton: public QPushButton
{
    Q_OBJECT
public:
    AttackButton(const BattleMove& b);
    void updateAttack(const BattleMove& b);

    QLabel *name;
    QLabel *pp;
};

class PokeButton;

/* When you want to switch pokemons, that's what you see */
class PokeZone : public QWidget
{
    Q_OBJECT
public:
    PokeZone(const TeamBattle &team);

    PokeButton *pokes[6];
signals:
    void switchTo(int poke);

private:
    QSignalMapper *mymapper;
};

class PokeButton : public QPushButton
{
    Q_OBJECT
public:
    PokeButton(const PokeBattle &p);
    void update();
private:

    const PokeBattle *p;
};


/* Yeepee, at last templates */
template <class T>
void GraphicsZone::switchTo(const T &poke, bool self, bool sub)
{
    if (self)
        mine->setPixmap(loadPixmap(poke.num(), poke.shiny(), true, poke.gender(), sub));
    else
        foe->setPixmap(loadPixmap(poke.num(), poke.shiny(), false, poke.gender(), sub));
}

#endif // BATTLEWINDOW_H

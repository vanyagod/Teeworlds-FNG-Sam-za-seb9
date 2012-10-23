/* (c) Timo Buhrmester. See licence.txt in the root of the distribution   */
/* for more information. If you are missing that file, acquire a complete */
/* release at teeworlds.com.                                              */

#ifndef GAME_SERVER_GAMEMODES_OPENFNG_H
#define GAME_SERVER_GAMEMODES_OPENFNG_H

#include <game/server/gamecontroller.h>

#define MAX_BROADCAST 256

#define MAX_SCOREDISPLAYS 3 // per team

// takes care of loltext teamscore displays
class CScoreDisplay
{
private:
	vec2 m_aScoreDisplays[2][MAX_SCOREDISPLAYS];
	int m_aScoreDisplayTextIDs[2][MAX_SCOREDISPLAYS];
	int m_aScoreDisplayCount[2];
	int m_aScoreDisplayValue[2];
	int m_Changed;
	class CGameContext *m_pGS;

	void FindMarkers();

public:
	CScoreDisplay(class CGameContext *pGameServer);
	virtual ~CScoreDisplay();

	void Reset(bool Destruct = false);
	void Add(int Team, vec2 Pos);
	void Update(int Team, int Score);
	void Operate();
};

// handles broadcasts
class CBroadcaster
{
private:
	char m_aBroadcast[MAX_CLIENTS][MAX_BROADCAST];
	int m_aNextBroadcast[MAX_CLIENTS];
	int m_aBroadcastStop[MAX_CLIENTS];
	char m_aDefBroadcast[MAX_BROADCAST];

	int m_Changed;

	class CGameContext *m_pGS;
public:
	CBroadcaster(class CGameContext *pGameServer);
	virtual ~CBroadcaster();

	void SetDef(const char *pText);
	void Update(int Cid, const char *pText, int Lifespan);
	void Reset();
	void Operate();
};

class CGameControllerOpenFNG : public IGameController
{
private:
	int m_aFrozenBy[MAX_CLIENTS];
	int m_aMoltenBy[MAX_CLIENTS];
	int m_aLastInteraction[MAX_CLIENTS]; //keep track of the last hostile interaction (hook/hammer), maps clientids to clientids [4] = 7 ^= cid 4 was last hooked/hammered by cid 7

	class CScoreDisplay m_ScoreDisplay;
	class CBroadcaster m_Broadcast;

	char m_aRagequitAddr[128];

	int m_aCltMask[2]; //for sending damageindicators only to teammates

	void SendFreezeKill(int Killer, int Victim, int Weapon);
	void HandleFreeze(int Killer, int Victim);
	void HandleSacr(int Killer, int Victim, int ShrineTeam);

	void Reset(bool Destruct = false);

	bool DoEmpty();
	void DoHookers(); //:P
	void DoInteractions();
	void DoBroadcasts(bool ForceSend = false);
	void DoScoreDisplays();
	void DoRagequit();
public:
	CGameControllerOpenFNG(class CGameContext *pGameServer);
	virtual ~CGameControllerOpenFNG();

	virtual bool CanBeMovedOnBalance(int ClientID);
	virtual void Tick();
	virtual void Snap(int SnappingClient);
	virtual bool OnEntity(int Index, vec2 Pos);
	virtual void OnCharacterSpawn(class CCharacter *pChr);
	virtual int OnCharacterDeath(class CCharacter *pVictim,
	                          class CPlayer *pUnusedKiller, int Weapon);
	//virtual void OnPlayerInfoChange(class CPlayer *pP);
	//virtual bool CanSpawn(int Team, vec2 *pPos);
	//virtual const char *GetTeamName(int Team);
	//virtual int GetAutoTeam(int NotThisID);
	virtual bool CanJoinTeam(int Team, int NotThisID);
	virtual void PostReset();
};
#endif

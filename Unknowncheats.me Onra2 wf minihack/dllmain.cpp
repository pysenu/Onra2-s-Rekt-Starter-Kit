// _                 _         _           _
//| |               | |       | |         | |
//| | _____   ____ _| | __    | |__   __ _| |_ ___ ___      _ __ ___   ___
//| |/ _ \ \ / / _` | |/ /    | '_ \ / _` | __/ _ / __|    | '_ ` _ \ / _ \
//| |  __/\ V | (_| |   <     | | | | (_| | ||  __\__ \    | | | | | |  __/
//|_|\___| \_/ \__,_|_|\_\    |_| |_|\__,_|\__\___|___/    |_| |_| |_|\___|

//made for unknowncheats
//credits: me (onra2)

#include "stdafx.h"
#include "Includes.h"
int ScreenWidth = 0;
int ScreenHeight = 0;
int ScreenCenterX = 0;
int ScreenCenterY = 0;
LPDIRECT3DDEVICE9 pGlobalDevice;
D3DVIEWPORT9 Viewport;
SSystemGlobalEnvironment* pSSGE = (SSystemGlobalEnvironment*)(*(DWORD*)(SsystemGlobalEnvironmentAdressEU));
IGameFramework* pFrameWork = IGameFramework::GetGameFramework();

typedef HRESULT(WINAPI* EndScene_)(LPDIRECT3DDEVICE9 pDevice);
EndScene_ pEndScene;
HMODULE OhModule;

static bool bReset = false;
static bool WH = true;//for mod menu later
static bool Silhouettes = true;
static bool aimbot = true;

#pragma region utilities
float Distance(Vec3 VecA, Vec3 VecB)
{
	return sqrt(((VecA.x - VecB.x) * (VecA.x - VecB.x)) +
		((VecA.y - VecB.y) * (VecA.y - VecB.y)) +
		((VecA.z - VecB.z) * (VecA.z - VecB.z)));
}

int GetTeam(IActor* Actor)
{
	IGameFramework* pFrameWork = IGameFramework::GetGameFramework();
	if (Actor)
		return pFrameWork->GetIGameRules()->GetTeam(Actor->getIEntity()->GetID());//pEnt->GetID()
}

bool MyTeam(IActor* MyPlayer, IActor* EnemyPlayer)
{
	int mTeam = GetTeam(MyPlayer);
	int pTeam = GetTeam(EnemyPlayer);

	if ((mTeam != pTeam || pTeam == 0))
		return true;
	else
		return false;
}

bool GetDeadPlayer(IActor* Player)
{
	if (Player->IsDead()) return true;
	else return false;
}

bool WorldToScreen(Vec3 vEntPos, Vec3 *sPos)
{
	IRenderer::w2s_info info;
	info.Posx = vEntPos.x;
	info.Posy = vEntPos.y;
	info.Posz = vEntPos.z;

	info.Scrnx = &sPos->x;
	info.Scrny = &sPos->y;
	info.Scrnz = &sPos->z;

	pSSGE->pRenderer->ProjectToScreen(&info);

	if (sPos->z < 0.0f || sPos->z > 1.0f) {
		return 0;
	}

	sPos->x *= (Viewport.Width / 100.0f);
	sPos->y *= (Viewport.Height / 100.0f);
	sPos->z *= 1.0f;

	return (sPos->z < 1.0f);
}

bool IsEnemy(IEntity* pLclEnt, IEntity* pEnt)
{
	IGameFramework* pFrameWork = IGameFramework::GetGameFramework();
	IGameRules* pGameRules = pFrameWork->GetIGameRules();

	int pTeam = pGameRules->GetTeam(pEnt->GetID());
	int pLclTeam = pGameRules->GetTeam(pLclEnt->GetID());

	return (pLclTeam != 0) ? (pTeam != pLclTeam) : 1;
}

Vec3 GetPlayerPos(IEntity* pEntit)
{
	Vec3 vOffset = Vec3();
	Matrix34 pWorld = pEntit->GetWorldTM();
	vOffset = pWorld.GetTranslation();
	return vOffset;
}

Vec3 GetBonePositionByID(IEntity* pEnt, int BoneID)
{
	char* getBoneName;
	ICharacterInstance *pCharacterInstance = pEnt->GetCharacter(0);
	ISkeletonPose *pSkeletonPose = pCharacterInstance->GetISkeletonPose();
	int sBoneID = BoneID;
	Matrix34 World = pEnt->GetWorldTM();
	Matrix34 SkeletonAbs = Matrix34(pSkeletonPose->GetBoneByID(sBoneID));
	Matrix34 matWorld = World;
	float m03, m13, m23;
	m03 = (World.m00*SkeletonAbs.m03) + (World.m01*SkeletonAbs.m13) + (World.m02*SkeletonAbs.m23) + World.m03;
	matWorld.m03 = m03;
	m13 = (World.m10*SkeletonAbs.m03) + (World.m11*SkeletonAbs.m13) + (World.m12*SkeletonAbs.m23) + World.m13;
	matWorld.m13 = m13;
	m23 = (World.m20*SkeletonAbs.m03) + (World.m21*SkeletonAbs.m13) + (World.m22*SkeletonAbs.m23) + World.m23;
	matWorld.m23 = m23;

	return matWorld.GetTranslation();
}

extern "C" void Aim(IActor* MyActor, IEntity* pEnt, Vec3 EnemyBone, float dist)
{
	if (!MyActor)
		return;
	if (!pEnt)
		return;
	Vec3 vDiffer;
	vDiffer = EnemyBone - GetBonePositionByID(MyActor->getIEntity(), 13);//13 is head pvp
	Quat FinalHeadPos = Quat::CreateRotationVDir(vDiffer.normalize());
	if (dist>0) { MyActor->GetPlayer()->SetViewRotation(FinalHeadPos); }
	else if (dist>10) { MyActor->GetPlayer()->SetViewRotation(FinalHeadPos); }
	else if (dist>20) { MyActor->GetPlayer()->SetViewRotation(FinalHeadPos); }
	else if (dist>30) { MyActor->GetPlayer()->SetViewRotation(FinalHeadPos); }
	else if (dist>40) { MyActor->GetPlayer()->SetViewRotation(FinalHeadPos); }
	else if (dist>50) { MyActor->GetPlayer()->SetViewRotation(FinalHeadPos); }
	else if (dist>60) { MyActor->GetPlayer()->SetViewRotation(FinalHeadPos); }
	else if (dist>70) { MyActor->GetPlayer()->SetViewRotation(FinalHeadPos); }
	else if (dist>80) { MyActor->GetPlayer()->SetViewRotation(FinalHeadPos); }
	else if (dist>90) { MyActor->GetPlayer()->SetViewRotation(FinalHeadPos); }
}

INT isVisible(Vec3 EnemyPos, Vec3 MyPos)
{
	Vec3 vTemp;
	ray_hit tmpHit;
	vTemp.x = EnemyPos.x - MyPos.x;
	vTemp.y = EnemyPos.y - MyPos.y;
	vTemp.z = EnemyPos.z - MyPos.z;
	return !pSSGE->pPhysicalWorld->RayWorldIntersection(MyPos, vTemp, 779, (10 & 0x0F), &tmpHit, 1);
}

int iFOV = 360;
#define PI    ((FLOAT)  3.141592654f)
bool InFOV(Vec3 EnemyPos, IActor* MyActor)
{
	SMovementState sState;
	MyActor->GetMovementController()->GetMovementState(sState);

	Vec3 vDir = EnemyPos - sState.pos;
	Vec3 AimDir = sState.aimDirection;
	vDir.Normalize();

	float fVal = acos(vDir.Dot(AimDir) / (AimDir.GetLengthSquared() * vDir.GetLengthSquared())) * (180 / PI);

	return (fVal <= iFOV);
}
#pragma endregion

void Hack()
{
	if (WH)
	{
		IActor * MyPlayer = NULL;
		IActor * EnemyPlayer = NULL;
		IEntitySystem * pEntSys = NULL;
		IEntityIt * pEntIt = NULL;
		IEntity * MyEntity = NULL;
		IEntity * EnemyEntity = NULL;
		IEntityRenderProxy *pEntityRenderProxy = NULL;
		IGameRules* pGameRules = NULL;

		if (!pFrameWork)
			return;

		pGameRules = pFrameWork->GetIGameRules();
		if (!pGameRules)
			return;

		pEntSys = pSSGE->pEntitySystem;
		if (!pEntSys)
			return;

		pEntIt = pEntSys->GetEntityIterator();
		if (!pEntIt)
			return;

		if (!pFrameWork->GetClientActor(&MyPlayer))
			return;

		if (!MyPlayer)
			return;

		for (; IEntity* pEnt = pEntIt->Next(); )
		{
			if (!pEnt)
				continue;

			if (EnemyPlayer = pFrameWork->GetIActorSystem()->GetActor(pEnt->GetID()))
			{
				if (MyPlayer != EnemyPlayer)
				{
					MyEntity = MyPlayer->getIEntity();
					if (!MyEntity)
						continue;

					EnemyEntity = EnemyPlayer->getIEntity();
					if (!EnemyEntity)
						continue;

					if (EnemyPlayer->IsDead())
						continue;

					if (Silhouettes)
					{
						pEntityRenderProxy = (IEntityRenderProxy*)(pEnt->GetProxy(ENTITY_PROXY_RENDER));
						if (!pEntityRenderProxy)
							continue;
						pEntityRenderProxy->SetRndFlags(eAllMap);
						pEntityRenderProxy->SetHUDSilhouettesParams(0, 0, 255, 0);
					}

					Vec3 EnemyPos = GetPlayerPos(pEnt);
					Vec3 Head = GetBonePositionByID(pEnt, 13), Head2;
					Vec3 pHead = GetBonePositionByID(pEnt, 18);
					Vec3 mHead = GetBonePositionByID(MyPlayer->getIEntity(), 18);
					Vec3 Mpos = GetPlayerPos(MyEntity);
					Vec3 EnemyBone{ 0, 0, 0 };
					Vec3 Out;

					if (MyTeam(MyPlayer, EnemyPlayer) && (!GetDeadPlayer(EnemyPlayer)))
					{
						if ((WorldToScreen(Head, &Head2)) && (WorldToScreen(EnemyPos, &Out) && (!GetDeadPlayer(EnemyPlayer))) && (MyTeam(MyPlayer, EnemyPlayer)))
						{
							float h = abs(Out.y - Head2.y);
							float w = h / 1.25f;
							float ScaledPosX = Head2.x + w / 2 + 5;
							float ScaledPosY = (Head2.y - (w / 2) + w) - 4;
							EnemyBone = GetBonePositionByID(pEnt, 13);//pvp
							float Distancion;
							Distancion = Distance(GetPlayerPos(MyEntity), GetPlayerPos(pEnt));
							if (!Distancion)
								continue;

							if ((InFOV(pHead, MyPlayer)) && isVisible(EnemyBone, mHead))
							{
								if (aimbot)
									if (GetAsyncKeyState(VK_RBUTTON))
									{
										if (!MyPlayer->IsDead())
										{
											Aim(MyPlayer, pEnt, EnemyBone, Distancion);
										}
									}
							}
							else
							{
								continue;
							}
						}
						else
						{
							continue;
						}
					}
					else
					{
						continue;
					}
				}
				else
				{
					continue;
				}
			}
			else
			{
				continue;
			}
		}
	}
}

static bool bWasOpen = false;
static bool Create = false;
HRESULT WINAPI EndScene(LPDIRECT3DDEVICE9 pDevice)
{
	if (!Create)
	{
		pDevice->GetViewport(&Viewport);
		ScreenWidth = Viewport.Width;
		ScreenHeight = Viewport.Height;
		ScreenCenterX = (ScreenWidth / 2);
		ScreenCenterY = (ScreenHeight / 2);
		Create = true;
	}
	Hack();
	return (*pEndScene)(pDevice);
}

DWORD FindDevice(DWORD Len)
{
	DWORD dwObjBase = 0;

	dwObjBase = (DWORD)LoadLibrary(L"d3d9.dll");
	while (dwObjBase++ < dwObjBase + Len)
	{
		if ((*(WORD*)(dwObjBase + 0x00)) == 0x06C7
			&& (*(WORD*)(dwObjBase + 0x06)) == 0x8689
			&& (*(WORD*)(dwObjBase + 0x0C)) == 0x8689
			) {
			dwObjBase += 2; break;
		}
	}
	return(dwObjBase);
}
DWORD GetDeviceAddress(int VTableIndex)
{
	PDWORD VTable;
	*(DWORD*)&VTable = *(DWORD*)FindDevice(0x128000);
	return VTable[VTableIndex];
}

DWORD WINAPI endhook(LPVOID pThreadParameter)
{
	DetourFunction((PBYTE)GetDeviceAddress(42), (PBYTE)&EndScene);
	__asm mov[pEndScene], eax;
	return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		OhModule = hModule;
		MessageBox(0, L"Cheat made by Onra2", L"Onra2", MB_ICONINFORMATION);
		CloseHandle(CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)endhook, NULL, NULL, NULL));
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}


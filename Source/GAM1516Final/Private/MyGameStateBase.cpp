// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameStateBase.h"


int AMyGameStateBase::GetPlayerHealth()
{
    return m_PlayerHealth;
}

int AMyGameStateBase::GetTilanHealth()
{
    return m_TilanHealth;
}

int AMyGameStateBase::GetAmmo()
{
    return m_Ammo;
}

void AMyGameStateBase::SetPlayerHealth(int health)
{
    m_PlayerHealth = health;
}

void AMyGameStateBase::SetTilanHealth(int health)
{
    m_TilanHealth = health;
}

void AMyGameStateBase::SetAmmo(int ammo)
{
    m_Ammo = ammo;
}

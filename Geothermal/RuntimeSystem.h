#pragma once
/// <summary>
/// systems should not contain any data, only methods
/// this base class is abstract
/// system is job (don't separate them into 2 concepts)
/// </summary>
class RuntimeSystem
{
	virtual void Update();
	virtual void LateUpdate();
};


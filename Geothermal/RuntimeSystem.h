#pragma once

/// <summary>
/// systems should not contain any data, only methods
/// this base class is abstract
/// system is job (don't separate them into 2 concepts)
/// </summary>
class RuntimeSystem
{
public:
	bool activated;

public:
	void PublicTickHandle();

	void PublicLateTickHandle();

protected:
	/// <summary>
	/// Override the update function to add custom logic.
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// Override late update function to add logic that's executed after every update function.
	/// </summary>
	virtual void LateUpdate() = 0;
};


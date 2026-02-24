#pragma once
#include <ScriptManager.h>

namespace Pixie
{
	class GameObject;
};

struct SplineGameComponent
{
public:
	virtual void Draw(Pixie::GameObject& selected) = 0;
	virtual std::string GetName() = 0;
	virtual void Remove(Pixie::GameObject& selected) = 0;
};

struct Attack : public SplineGameComponent
{
	int Damage{ 1 };

	virtual std::string GetName() override { return "Attack Component"; }
	virtual void Draw(Pixie::GameObject& selected) override;
	virtual void Remove(Pixie::GameObject& selected) override;
};

struct ScoresPoints : public SplineGameComponent
{
	int Value{ 1 };

	virtual std::string GetName() override { return "Point Value Component"; }
	virtual void Draw(Pixie::GameObject& selected) override;
	virtual void Remove(Pixie::GameObject& selected) override;
};

#pragma once

namespace BarnabusGameEngine
{
	class BarnabusGameEngine
	{
	private:
		bool InitialiseGameEngine();
	protected:
		bool LoadGameContent();
		bool Update(double deltaTime);
		bool Render(double deltaTime);
	public:
		bool StartGameEngine();

	};
}
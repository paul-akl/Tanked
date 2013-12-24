#pragma once
namespace Utils
{
	class GeneralUtils
	{
	public:
		GeneralUtils(void);
		static void exitFatalError(const char *message);

		template <class T> static const T& max (const T& a, const T& b) {
		 return (a<b)?b:a;    
		}

		static bool timerElapsed(int currentTimer, int duration)
		{
			if (currentTimer >= duration)
				return true;
			return false;
		}

		~GeneralUtils(void);
	};
};

#ifndef TCSnapPage_h
#define TCSnapPage_h

#include "TCPage.h"
#include "CppTweener.h"

using namespace tween;

namespace Touched {
	class TCSnapPage : public TCPage, public TweenerListener {
	protected:
		long milli_seconds;
		Tweener tweener;
		TweenerParam param;
		
		int current_index;
		
		virtual void CalculateMovement();
		
	public:
		TCSnapPage();
		~TCSnapPage();
		
		virtual void TouchEndOrCancel(void* _address, float _position);
		
		void GoPage(int _index, short ptransition = EXPO);
		
		int GetCurrentIndex() {
			return current_index;
		}
		
		virtual void ChangedIndex(int _index) = 0;
		
		virtual void onStart(TweenerParam& param);
		virtual void onStep(TweenerParam& param);
		virtual void onComplete(TweenerParam& param);
	};
}


#endif

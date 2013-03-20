//
//  TCMovementTab.h
//  touched_cpp
//
//  Created by wonhee jang on 13. 3. 20..
//  Copyright (c) 2013년 vanillabreeze. All rights reserved.
//

#ifndef touched_cpp_TCMovementTab_h
#define touched_cpp_TCMovementTab_h

#include "CppTweener.h"

using namespace tween;

namespace Touched {
	class TCMovementTab : public TweenerListener {
	protected:
		long milliseconds;
		Tweener tweener;
		TweenerParam param;
		
		float width;
		float x;
		float dir_width;
		float dir_x;
		int max_index;
		int index;
		
	public:
		TCMovementTab(float _width, int _maxidx, int _idx);
		virtual ~TCMovementTab();
		
		void Update(float _tick);
		
		void SetWidth(float _width);
		float GetWidth();
		
		void SetMaxIndex(int _maxidx);
		int GetMaxIndex();
		
		void SetIndex(int _idx);
		int GetIndex();
		
		void TweenStart();
		
		virtual void onStart(TweenerParam& param);
		virtual void onStep(TweenerParam& param);
		virtual void onComplete(TweenerParam& param);
	};
}



#endif

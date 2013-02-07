#include "TCSnapPage.h"
#include <float.h>

#define SLIDE_TOUCH_TH 5

using namespace Touched;

TCSnapPage::TCSnapPage() : TCPage(), TweenerListener() {
	current_index = 0;
}

TCSnapPage::~TCSnapPage() {
}

void TCSnapPage::CalculateMovement() {
	if(touch_address) {
		position += force;
		force = 0;
	} else {
		//tween
		milli_seconds += 1.0/60.0 * 1000;
		tweener.step(milli_seconds);
	}
}

void TCSnapPage::GoPage(int _index, short ptransition) {
	if(_index != current_index) {
		current_index = _index;
		ChangedIndex(current_index);
	}
	if(current_index < 0)
		current_index = 0;
	if(current_index > GetPageLength() - 1)
		current_index = GetPageLength() - 1;
	
	float _size = 0.0;
	for(int i = 0; i < current_index; i++) {
		_size += GetPageSize(0);
	}
	_size -= GetPageSize(current_index) * 0.5;
	_size += GetSize() * 0.5;
	
	milli_seconds = 0;
	tweener = Tweener();
	param = TweenerParam(600, ptransition, EASE_OUT);
	param.addProperty(&position, -_size);
	tweener.addTween(param);
}

void TCSnapPage::TouchEndOrCancel(void* _address, float _position) {
	if(touch_address == _address) {
		touch_address = NULL;
		
		int nextIdx = current_index;
        if(fabsf(force_delta) > SLIDE_TOUCH_TH) {
            if(force_delta < 0) {
                if(nextIdx < GetPageLength() - 1)
                    nextIdx++;
            } else {
                if(nextIdx > 0)
                    nextIdx--;
            }
			GoPage(nextIdx, QUART);
        } else {
			std::list<Page*>::iterator _it = page_list.begin();
			float _half = GetSize() * 0.5;
			while(page_list.end() != _it) {
				Page* _page = *_it;
				if(_half > _page->position && _half < _page->position + _page->size) {
					nextIdx = _page->number;
					break;
				}
				_it++;
			}
			GoPage(nextIdx, QUART);
		}
	}
}

void TCSnapPage::onStart(TweenerParam& param) {
	
}

void TCSnapPage::onStep(TweenerParam& param) {
	
}

void TCSnapPage::onComplete(TweenerParam& param) {
	position = param.properties[0].finalValue;
}
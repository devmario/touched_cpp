#include "TCPage.h"
#include <math.h>
#include <sys/time.h>
#ifdef ANDROID_NDK
#include <iostream>
#else
#include <iostream>
#endif

using namespace Touched;

TCPage::Page* TCPage::GetPage(int _page_number) {
	std::list<Page*>::iterator _it = page_list.begin();
	while(page_list.end() != _it) {
		Page* _page = *_it;
		if(_page->number == _page_number)
			return _page;
		_it++;
	}
	return NULL;
}

TCPage::TCPage() {
	is_center = false;
	width_left_margin = 0;
	width_right_margin = 0;
	enable = true;
	is_movement = true;
	enable_rollback_scroll = true;
	Init();
}

void TCPage::Init() {
	touch_address = NULL;
	touch_position = 0.0;
	
	position = width_left_margin;
	force = 0.0;
	
	size = 0.0;
	
	step = width_left_margin;
	start_index = 0;
	
	x_translate = 0;
}

TCPage::~TCPage() {
	Clean();
}

void TCPage::Clean() {
	std::list<Page*>::iterator _it = page_list.begin();
	while(page_list.end() != _it) {
		Page* _page = *_it;
		FreePage(_page->address);
		delete _page;
		_it++;
	}
	page_list.clear();
}

void TCPage::ReloadData() {
	Clean();
	Update();
}

void TCPage::Reset() {
	Init();
	ReloadData();
}

void TCPage::UpdateCell(int _page_num) {
	std::list<Page*>::iterator _it = page_list.begin();
	while(page_list.end() != _it) {
		Page* _page = *_it;
		if(_page->number == _page_num) {
			if(_page->address) {
				FreePage(_page->address);
				_page->address = AllocPage(_page_num);
				UpdatePage(_page->address, _page->position);
			} else {
				_page->address = AllocPage(_page_num);
				UpdatePage(_page->address, _page->position);
			}
		}
		_it++;
	}
}

void TCPage::CalculateSize() {
	//전체 사이즈 계산
	size = 0.0;
	for(int i = 0; i < GetPageLength(); i++) {
		size += GetPageSize(i);
	}
}

void TCPage::CalculateMovement() {
	if(!is_movement)
		return;
	//물리움직임
	position += force;
	if(touch_address) {
		//터치시
		force = 0.0;
	} else {
		//터치 땠을때
		float _w_avail = GetSize() - width_left_margin - width_right_margin;
		if(is_center && size < _w_avail) {
			position += (GetSize() * 0.5 - size * 0.5  - position) * 0.2;
			force *= 0.9;
		} else {
			if(position > width_left_margin) {
				//처음리스트에서 더욱더 스크롤했을시
				if(enable_rollback_scroll)
					position += (width_left_margin - position) * 0.2;
				force *= 0.9;
			} else if(size > _w_avail) {
				if(position < -size + GetSize() - width_right_margin) {
					//마지막리스트에서 더욱더 스크롤했을시
					if(enable_rollback_scroll)
						position += (-size + GetSize() - width_right_margin - position) * 0.2;
					force *= 0.9;
				} else {
					force *= 0.95;
				}
			} else {
				if(position < -size + GetSize() - width_right_margin) {
					//마지막리스트에서 더욱더 스크롤했을시
					if(enable_rollback_scroll)
						position += (width_left_margin - position) * 0.2;
					force *= 0.9;
				} else {
					force *= 0.95;
				}
			}
		}
	}
}

#define ALLOCATED_MARGIN 0.5//For PageSnap

void TCPage::CalculateStartIndex(float _change_position) {
	//start_index계산
	step += _change_position;
	if(step - ALLOCATED_MARGIN < -GetPageSize(start_index)) {
		while (step - ALLOCATED_MARGIN < -GetPageSize(start_index)) {
			step += GetPageSize(start_index);
			start_index++;
		}
	} else if(step - ALLOCATED_MARGIN > 0.0) {
		while (step - ALLOCATED_MARGIN > 0.0) {
			start_index--;
			step -= GetPageSize(start_index);
		}
	}
}

void TCPage::CalculateAllocPage() {
	int i = start_index;
	float _current_position = step + ALLOCATED_MARGIN;
	
	//생성(페이지 사이즈안에 들어오면)
	while(_current_position < GetSize() - ALLOCATED_MARGIN) {
		Page* _page = GetPage(i);
		if(_page == NULL) {
			_page = new Page;
			_page->number = i;
			_page->position = _current_position - ALLOCATED_MARGIN;
			_page->size = GetPageSize(i);
			_page->address = AllocPage(i);
			_page->moved = true;
			page_list.push_back(_page);
		}
		
		_current_position += GetPageSize(i);
		i++;
	}
}
void TCPage::CalculateFreeOrUpdatePage(float _change_position) {
	//삭제(페이지 사이즈에서 벗어나면) 또는 업데이트
	std::list<Page*>::iterator _it = page_list.begin();
	while(page_list.end() != _it) {
		Page* _page = *_it;
		_it++;
		
		if(!_page->moved) {
			_page->position += _change_position;
			_page->moved = true;
		}
		
		if(_page->position + GetPageSize(_page->number) < ALLOCATED_MARGIN ||
		   _page->position > GetSize() - ALLOCATED_MARGIN) {
			
			FreePage(_page->address);
			page_list.remove(_page);
			delete _page;
			
		} else {
			if(_page->moved) {
				UpdatePage(_page->address, _page->position);
				_page->moved = false;
			}
		}
	}
}

void TCPage::Update() {
	CalculateSize();
	
	//이전위치
	float prev_position = position;
	
	CalculateMovement();
	
	float _change_position = position - prev_position + x_translate;
	position += x_translate;
	x_translate = 0;
	
	CalculateStartIndex(_change_position);
	
	CalculateAllocPage();
	CalculateFreeOrUpdatePage(_change_position);
}

void TCPage::TouchBegin(void* _address, float _position) {
	if(!enable) {
		if(touch_address) {
			force = force_delta;
			touch_address = NULL;
			return;
		}
	}
	if(touch_address == NULL) {
		touch_address = _address;
		touch_position = _position;
		force = 0.0;
		
		gettimeofday(&cur_time, NULL);
		force_delta = 0.0;
	}
}

void TCPage::TouchMove(void* _address, float _position) {
	if(!enable) {
		if(touch_address) {
			force = force_delta;
			touch_address = NULL;
			return;
		}
	}
	if(touch_address == _address) {
		force += _position - touch_position;
		touch_position = _position;
		
		struct timeval next_time;
		gettimeofday(&next_time, NULL);
		
		//프레임별 경과시간
		float _delta = (((next_time.tv_sec * 1000000.0) + next_time.tv_usec) - ((cur_time.tv_sec * 1000000.0) + cur_time.tv_usec)) / 1000000.0;
		if(_delta < 0.01)
			_delta = 0.01;
		force_delta = force / _delta * 0.02;
		
		cur_time = next_time;
	}
}

void TCPage::TouchEndOrCancel(void* _address, float _position) {
	if(!enable) {
		if(touch_address) {
			force = force_delta;
			touch_address = NULL;
			return;
		}
	}
	if(touch_address == _address) {
		force = force_delta;
		touch_address = NULL;
	}
}
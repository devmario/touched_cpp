//
//  TCMovementTab.cpp
//  touched_cpp
//
//  Created by wonhee jang on 13. 3. 20..
//  Copyright (c) 2013년 vanillabreeze. All rights reserved.
//

#include "TCMovementTab.h"

using namespace Touched;

TCMovementTab::TCMovementTab(float _width, int _maxidx, int _idx) {
	dir_width = width = _width;
	max_index = _maxidx;
	index = _idx;
	dir_x = x = 0;
}

TCMovementTab::~TCMovementTab() {
	
}

void TCMovementTab::Update(float _tick) {
	milliseconds += _tick * 1000;
	tweener.step(milliseconds);
}

void TCMovementTab::TCSetWidth(float _width) {
	if(dir_width != _width) {
		dir_width = _width;
		TweenStart();
	}
}

float TCMovementTab::TCGetWidth() {
	return width;
}

void TCMovementTab::SetMaxIndex(int _maxidx) {
	SetIndex(GetIndex());
}

int TCMovementTab::GetMaxIndex() {
	return max_index;
}

void TCMovementTab::SetIndex(int _idx) {
	if(index != _idx) {
		index = _idx;
		dir_x = (float)index / (float)max_index * (float)dir_width;
		TweenStart();
	}
}

int TCMovementTab::GetIndex() {
	return index;
}

float TCMovementTab::GetX() {
	return x;
}

void TCMovementTab::TweenStart() {
	milliseconds = 0;
	tweener = Tweener();
	param = TweenerParam(1000*0.5, EXPO, EASE_OUT);
	param.addProperty(&x, dir_x);
	param.addProperty(&width, dir_width);
	tweener.addTween(param);
}

void TCMovementTab::onStart(TweenerParam& param) {
	
}

void TCMovementTab::onStep(TweenerParam& param) {
	
}

void TCMovementTab::onComplete(TweenerParam& param) {
	
}
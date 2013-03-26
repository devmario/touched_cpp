#include "TCScene.h"

using namespace Touched;

TCScene::TCScene(const std::string _name) {
	name = _name;
	
	loaded = false;
	need_load = false;
	is_reload = false;
	pre_load = false;
	pre_unload = false;
	need_reload = false;
	
	TCSceneManager::Share()->scene_list.push_back(this);
}

TCScene::~TCScene() {
	TCSceneManager::Share()->scene_list.remove(this);
}

std::string TCScene::GetName() {
	return name;
}

bool TCScene::GetIsLoadedScene() {
	return loaded || need_load;
}

void TCScene::Update(bool _for_load) {
	if(need_reload) {
		if(!_for_load) {
			if(pre_unload) {
				PreUnload();
				pre_unload = false;
			}
			if(ReadyUnload()) {
				Unload();
				loaded = false;
				need_reload = false;
			}
		}
	} else if(loaded != need_load) {
		if(_for_load) {
			if(need_load) {
				if(pre_load) {
					PreLoad(value, is_reload);
					pre_load = false;
				}
				if(ReadyLoad(is_reload)) {
					Load(value, is_reload);
					loaded = need_load;
				}
			}
		} else {
			if(!need_load)  {
				if(pre_unload) {
					PreUnload();
					pre_unload = false;
				}
				if(ReadyUnload()) {
					Unload();
					loaded = need_load;
				}
			}
		}
	}
}

const Json::Value TCScene::GetValue() {
	return value;
}

TCSceneManager* __tc_scene_manager = NULL;

TCSceneManager* TCSceneManager::Share() {
	if(__tc_scene_manager == NULL) {
		__tc_scene_manager = new TCSceneManager();
	}
	return __tc_scene_manager;
}

TCScene* TCSceneManager::GetScene(const std::string _name) {
	std::list<TCScene*>::iterator _iterator = scene_list.begin();
	while(_iterator != scene_list.end()) {
		if((*_iterator)->name == _name)
			return *_iterator;
		_iterator++;
	}
	return NULL;
}

std::string TCSceneManager::GetCurrentSceneName()
{
	if(value_list.size() == 0)
		return "";
	return value_list.back()["name"].asString();
}

TCScene* TCSceneManager::GetCurrentScene()
{
	return GetScene(GetCurrentSceneName());
}

void TCSceneManager::Link(const Json::Value _value) {
	value_list.push_back(_value);
	
	std::list<TCScene*>::iterator _iterator = scene_list.begin();
	while(_iterator != scene_list.end()) {
		if((*_iterator)->loaded) {
			(*_iterator)->pre_unload = true;
		}
		(*_iterator)->need_load = false;
		_iterator++;
	}
	
	for(int _i = 0; _i < _value.size(); _i++) {
		std::string _name = _value[_i]["name"].asString();
		TCScene* _scene = GetScene(_name);
		_scene->value = _value[_i];
		if(_scene->loaded)
			_scene->is_reload = true;
		else
			_scene->is_reload = false;
		_scene->pre_load = _scene->need_load = true;
	}
}

bool TCSceneManager::Back() {
	if(value_list.size() > 1) {
		value_list.pop_back();
		std::list<Json::Value>::iterator it = value_list.end();
		it--;
		Json::Value value = *it;
//		std::cout << value.toStyledString();
		value_list.pop_back();
		Link(value);
        return true;
	} else {
        return false;
    }
}

void TCSceneManager::Reload() {
	if(!value_list.empty()) {
		Json::Value _value_last = value_list.back();
		for(int i = 0; i < _value_last.size(); i++) {
			std::string _name = _value_last[i]["name"].asString();
			
			
			std::list<TCScene*>::iterator _iterator = scene_list.begin();
			while(_iterator != scene_list.end()) {
				if(_name == (*_iterator)->GetName()) {
					(*_iterator)->need_reload = true;
					(*_iterator)->pre_unload = true;
					(*_iterator)->pre_load = true;
				}
				_iterator++;
			}
		}
	}
}

void TCSceneManager::ClearHistory() {
	if(value_list.size() > 0) {
		Json::Value value = *(value_list.end()--);
		value_list.clear();
		Link(value);
	}
}

//bool _a[3][4];

void TCSceneManager::Update() {
	std::list<TCScene*>::iterator _iterator = scene_list.begin();
	while(_iterator != scene_list.end()) {
		(*_iterator)->Update(false);
		_iterator++;
	}
	_iterator = scene_list.begin();
	while(_iterator != scene_list.end()) {
		(*_iterator)->Update(true);
		_iterator++;
	}
	_iterator = scene_list.begin();
	
//	bool a[3][4];
//	int n = 0;
//	while(_iterator != scene_list.end()) {
//		a[n][0] = (*_iterator)->need_load;
//		a[n][1] = (*_iterator)->loaded;
//		a[n][2] = (*_iterator)->pre_load;
//		a[n][3] = (*_iterator)->pre_unload;
//		_iterator++;
//		n++;
//	}
//	bool* __a = &_a[0][0];
//	bool* __b = &a[0][0];
//	for(int i = 0; i < 3*4; i++) {
//		if(*__a != *__b) {
//			memcpy(_a, a, sizeof(bool) * 4 * 3);
//			for(int j = 0; j < 3; j++) {
//				std::cout << "(" << a[j][0] << "," << a[j][1] << "," << a[j][2] << "," << a[j][3] << "),";
//			}
//			std::cout << "\n";
//			break;
//		}
//		__a++;
//		__b++;
//	}
}

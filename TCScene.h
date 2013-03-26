#ifndef TCScene_h
#define TCScene_h

#include <iostream>
#include <list>
#include <json/json.h>

namespace Touched {
	class TCScene {
	private:
		std::string name;
		Json::Value value;
		
		bool need_load;
		bool need_reload;
		bool pre_load;
		bool pre_unload;
		bool is_reload;
		
		void Update(bool _for_load);
		
	public:
		bool loaded;
		TCScene(const std::string _name);
		virtual ~TCScene();
		
		std::string GetName();
		
		bool GetIsLoadedScene();
		
		virtual void PreLoad(const Json::Value _value, bool _is_reload) = 0;
		virtual bool ReadyLoad(bool _is_reload) = 0;
		virtual void Load(const Json::Value _value, bool _is_reload) = 0;
		
		virtual void PreUnload() = 0;
		virtual bool ReadyUnload() = 0;
		virtual void Unload() = 0;
		
		const Json::Value GetValue();
		
		friend class TCSceneManager;
	};
	
	class TCSceneManager {
	private:
		std::list<TCScene*> scene_list;
		std::list<Json::Value> value_list;
		
	public:
		static TCSceneManager* Share();
		
		TCScene* GetScene(const std::string _name);
		
		std::string GetCurrentSceneName();
		TCScene* GetCurrentScene();
		
		void Link(const Json::Value _value);
		bool Back();
		void Reload();
		void ClearHistory();
		
		void Update();
		
		friend class TCScene;
	};
}

#endif
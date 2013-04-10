#ifndef TCPage_h
#define TCPage_h

#include <list>
#include <sys/time.h>

#ifndef NULL
#define NULL 0
#endif

namespace Touched {
	class TCPage {
	protected:
		struct timeval cur_time;
		float force_delta;
		
		void* touch_address;
		float touch_position;
		
		float force;
		
		/* 다음 3개의 속성들은 scroller도 써야함*/
		
		//위치
		float position;
		
		float GetVisiblePosition() {
			if(size > 0.0)
				return (-position+width_left_margin) / size;
			else
				return (-position+width_left_margin) / GetSize();
		};
		
		//전체사이즈
		float size;
		
		//화면에 보이는 사이즈(비율임)
		//(GetSize() - width_left_margin - width_right_margin) / size
		float GetVisiblePer() {
			if((GetSize()  - width_left_margin - width_right_margin) > size)
				return 1.0;
			else
				return (GetSize() - width_left_margin - width_right_margin) / size;
		}
		
		float step;
		int start_index;
		
		bool is_center;
		float width_left_margin;
		float width_right_margin;
		bool enable;
		bool is_movement;
		float x_translate;
		
		bool enable_rollback_scroll;
		
		typedef struct Page {
			void* address;
			float position;
			float size;
			int number;
			bool moved;
		} Page;
		std::list<Page*> page_list;
		
		Page* GetPage(int _page_number);
		
		void CalculateSize();
		virtual void CalculateMovement();
		virtual void CalculateStartIndex(float _change_position);
		void CalculateAllocPage();
		void CalculateFreeOrUpdatePage(float _change_position);
		
	public:
		TCPage();
		virtual ~TCPage();
		
		void Init();
		
		virtual void Clean();
		
		virtual int GetPageLength()=0;
		
		virtual float GetPageSize(int _page_number)=0;
		
		virtual float GetSize()=0;
		
		virtual void* AllocPage(int _page_number)=0;
		
		virtual void FreePage(void* _address)=0;
		
		virtual void UpdatePage(void* _address, float _position)=0;
		
		void ReloadData();
		void Reset();
		void UpdateCell(int _page_num);
		
		void SetForce(float _force) {
			force = _force;
		}
		
		void SetIsMovement(bool _is_movement) {
			is_movement = _is_movement;
		}
		
		void TranslateX(float _x) {
			x_translate += _x;
		}
		
		virtual void Update();
		
		virtual void TouchBegin(void* _address, float _position);
		virtual void TouchMove(void* _address, float _position);
		virtual void TouchEndOrCancel(void* _address, float _position);
		
		void SetEnable(bool _enable) {
			enable = _enable;
		}
		bool GetEnable() {
			return enable;
		}
		
		void SetWithRightMargin(float _margin) {
			width_right_margin = _margin;
		}
		
		void SetWithLeftMargin(float _margin) {
			width_left_margin = _margin;
		}
		
		void SetEnableRollbackScroll(float _enable) {
			enable_rollback_scroll = _enable;
		}
		
//		virtual void FetchRefresh(bool _dir, float _width) {};
//		virtual void UnfetchRefresh(float _l_width, float _r_width) {};
	};
}

#endif
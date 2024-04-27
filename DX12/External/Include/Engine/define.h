#pragma once

#define Single(classtype) private:\
						  classtype();\
						  ~classtype();\
						  friend class Singleton<classtype>;

#define DEVICE Device::GetInst()->GetDevice()
#define CMDLIST Device::GetInst()->GetCmdList()

#define PATH PathMgr::GetInst()->GetPath()

#define CLONE(TYPE)	virtual TYPE* Clone(){return new TYPE(*this);}
#define CLONE_DISABLE(TYPE) TYPE* Clone(){return nullptr; assert(nullptr);}\
							TYPE(const TYPE& _originBuffer) = delete;

#define ASPECT_RATIO Device::GetInst()->GetRes().x/ Device::GetInst()->GetRes().y

#define KEY_CHECK(Key, State) KeyMgr::GetInst()->GetKeyState(Key) == State
#define KEY_TAP(Key) KEY_CHECK(Key, TAP)
#define KEY_PRESSED(Key) KEY_CHECK(Key, PRESSED)
#define KEY_RELEASED(Key) KEY_CHECK(Key, RELEASED)
#define KEY_NONE(Key) KEY_CHECK(Key, NONE)

#define DT TimeMgr::GetInst()->GetDT()
#pragma once
#include "acl_cpp/acl_cpp_define.hpp"
#include "acl_cpp/master/master_conf.hpp"

struct ACL_EVENT;

namespace acl
{

class event_timer;

class ACL_CPP_API master_base
{
public:
	/**
	 * 设置 bool 类型的配置项
	 * @param table {master_bool_tbl*}
	 */
	void set_cfg_bool(master_bool_tbl* table);

	/**
	 * 设置 int 类型的配置项
	 * @param table {master_int_tbl*}
	 */
	void set_cfg_int(master_int_tbl* table);

	/**
	 * 设置 int64 类型的配置项
	 * @param table {master_int64_tbl*}
	 */
	void set_cfg_int64(master_int64_tbl* table);

	/**
	 * 设置 字符串 类型的配置项
	 * @param table {master_str_tbl*}
	 */
	void set_cfg_str(master_str_tbl* table);

	/**
	 * 判断是否是由 acl_master 控制的 daemon 模式
	 * @return {bool}
	 */
	bool daemon_mode(void) const;

	/////////////////////////////////////////////////////////////////////
	
	/**
	 * 设置进程级别的定时器，该函数只可在主线程的运行空间 (如在函数
	 * proc_on_init) 中被设置，当该定时器任务都执行完毕后会自动被
	 * 销毁(即内部会自动调用 master_timer::destroy 方法)
	 * @param timer {event_timer*} 定时任务
	 * @return {bool} 设置定时器是否成功
	 */
	bool proc_set_timer(event_timer* timer);

	/**
	 * 删除进程级别定时器
	 * @param timer {event_timer*} 由 proc_set_timer 设置的定时任务
	 */
	void proc_del_timer(event_timer* timer);

protected:
	bool daemon_mode_;
	bool proc_inited_;

	master_base();
	virtual ~master_base();

	/**
	 * 当进程切换用户身份前调用的回调函数，可以在此函数中做一些
	 * 用户身份为 root 的权限操作
	 */
	virtual void proc_pre_jail() {}

	/**
	 * 当进程切换用户身份后调用的回调函数，此函数被调用时，进程
	 * 的权限为普通受限级别
	 */
	virtual void proc_on_init() {}

	/**
	 * 当进程退出前调用的回调函数
	 */
	virtual void proc_on_exit() {}

	// 配置对象
	master_conf conf_;

protected:
	// 子类必须调用本方法设置事件引擎句柄
	void set_event(ACL_EVENT* event);

private:
	ACL_EVENT* event_;
};

}  // namespace acl

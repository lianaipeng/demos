// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************
//
// Ice version 3.6.1
//
// <auto-generated>
//
// Generated from file `AMIDemo.ice'
//
// Warning: do not edit this file.
//
// </auto-generated>
//

#ifndef __AMIDemo_h__
#define __AMIDemo_h__

#include <IceUtil/PushDisableWarnings.h>
#include <Ice/ProxyF.h>
#include <Ice/ObjectF.h>
#include <Ice/Exception.h>
#include <Ice/LocalObject.h>
#include <Ice/StreamHelpers.h>
#include <Ice/Proxy.h>
#include <Ice/GCObject.h>
#include <Ice/AsyncResult.h>
#include <Ice/Incoming.h>
#include <IceUtil/ScopedArray.h>
#include <IceUtil/Optional.h>
#include <Ice/StreamF.h>
#include <IceUtil/UndefSysMacros.h>

#ifndef ICE_IGNORE_VERSION
#   if ICE_INT_VERSION / 100 != 306
#       error Ice version mismatch!
#   endif
#   if ICE_INT_VERSION % 100 > 50
#       error Beta header file detected
#   endif
#   if ICE_INT_VERSION % 100 < 1
#       error Ice patch level mismatch!
#   endif
#endif

namespace IceProxy
{

namespace Demo
{

class MyInterface;
void __read(::IceInternal::BasicStream*, ::IceInternal::ProxyHandle< ::IceProxy::Demo::MyInterface>&);
::IceProxy::Ice::Object* upCast(::IceProxy::Demo::MyInterface*);

}

}

namespace Demo
{

class MyInterface;
bool operator==(const MyInterface&, const MyInterface&);
bool operator<(const MyInterface&, const MyInterface&);
::Ice::Object* upCast(::Demo::MyInterface*);
typedef ::IceInternal::Handle< ::Demo::MyInterface> MyInterfacePtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::Demo::MyInterface> MyInterfacePrx;
void __patch(MyInterfacePtr&, const ::Ice::ObjectPtr&);

}

namespace Demo
{

class Callback_MyInterface_op_Base : virtual public ::IceInternal::CallbackBase { };
typedef ::IceUtil::Handle< Callback_MyInterface_op_Base> Callback_MyInterface_opPtr;

}

namespace IceProxy
{

namespace Demo
{

class MyInterface : virtual public ::IceProxy::Ice::Object
{
public:

    ::Ice::Int op(::Ice::Double __p_d, const ::std::string& __p_s, ::Ice::Int& __p_i, ::Ice::Long& __p_l)
    {
        return op(__p_d, __p_s, __p_i, __p_l, 0);
    }
    ::Ice::Int op(::Ice::Double __p_d, const ::std::string& __p_s, ::Ice::Int& __p_i, ::Ice::Long& __p_l, const ::Ice::Context& __ctx)
    {
        return op(__p_d, __p_s, __p_i, __p_l, &__ctx);
    }
#ifdef ICE_CPP11
    ::Ice::AsyncResultPtr
    begin_op(::Ice::Double __p_d, const ::std::string& __p_s, const ::IceInternal::Function<void (::Ice::Int, ::Ice::Int, ::Ice::Long)>& __response, const ::IceInternal::Function<void (const ::Ice::Exception&)>& __exception = ::IceInternal::Function<void (const ::Ice::Exception&)>(), const ::IceInternal::Function<void (bool)>& __sent = ::IceInternal::Function<void (bool)>())
    {
        return __begin_op(__p_d, __p_s, 0, __response, __exception, __sent);
    }
    ::Ice::AsyncResultPtr
    begin_op(::Ice::Double __p_d, const ::std::string& __p_s, const ::IceInternal::Function<void (const ::Ice::AsyncResultPtr&)>& __completed, const ::IceInternal::Function<void (const ::Ice::AsyncResultPtr&)>& __sent = ::IceInternal::Function<void (const ::Ice::AsyncResultPtr&)>())
    {
        return begin_op(__p_d, __p_s, 0, ::Ice::newCallback(__completed, __sent), 0);
    }
    ::Ice::AsyncResultPtr
    begin_op(::Ice::Double __p_d, const ::std::string& __p_s, const ::Ice::Context& __ctx, const ::IceInternal::Function<void (::Ice::Int, ::Ice::Int, ::Ice::Long)>& __response, const ::IceInternal::Function<void (const ::Ice::Exception&)>& __exception = ::IceInternal::Function<void (const ::Ice::Exception&)>(), const ::IceInternal::Function<void (bool)>& __sent = ::IceInternal::Function<void (bool)>())
    {
        return __begin_op(__p_d, __p_s, &__ctx, __response, __exception, __sent);
    }
    ::Ice::AsyncResultPtr
    begin_op(::Ice::Double __p_d, const ::std::string& __p_s, const ::Ice::Context& __ctx, const ::IceInternal::Function<void (const ::Ice::AsyncResultPtr&)>& __completed, const ::IceInternal::Function<void (const ::Ice::AsyncResultPtr&)>& __sent = ::IceInternal::Function<void (const ::Ice::AsyncResultPtr&)>())
    {
        return begin_op(__p_d, __p_s, &__ctx, ::Ice::newCallback(__completed, __sent));
    }
    
private:

    ::Ice::AsyncResultPtr __begin_op(::Ice::Double __p_d, const ::std::string& __p_s, const ::Ice::Context* __ctx, const ::IceInternal::Function<void (::Ice::Int, ::Ice::Int, ::Ice::Long)>& __response, const ::IceInternal::Function<void (const ::Ice::Exception&)>& __exception, const ::IceInternal::Function<void (bool)>& __sent);
    
public:
#endif

    ::Ice::AsyncResultPtr begin_op(::Ice::Double __p_d, const ::std::string& __p_s)
    {
        return begin_op(__p_d, __p_s, 0, ::IceInternal::__dummyCallback, 0);
    }

    ::Ice::AsyncResultPtr begin_op(::Ice::Double __p_d, const ::std::string& __p_s, const ::Ice::Context& __ctx)
    {
        return begin_op(__p_d, __p_s, &__ctx, ::IceInternal::__dummyCallback, 0);
    }

    ::Ice::AsyncResultPtr begin_op(::Ice::Double __p_d, const ::std::string& __p_s, const ::Ice::CallbackPtr& __del, const ::Ice::LocalObjectPtr& __cookie = 0)
    {
        return begin_op(__p_d, __p_s, 0, __del, __cookie);
    }

    ::Ice::AsyncResultPtr begin_op(::Ice::Double __p_d, const ::std::string& __p_s, const ::Ice::Context& __ctx, const ::Ice::CallbackPtr& __del, const ::Ice::LocalObjectPtr& __cookie = 0)
    {
        return begin_op(__p_d, __p_s, &__ctx, __del, __cookie);
    }

    ::Ice::AsyncResultPtr begin_op(::Ice::Double __p_d, const ::std::string& __p_s, const ::Demo::Callback_MyInterface_opPtr& __del, const ::Ice::LocalObjectPtr& __cookie = 0)
    {
        return begin_op(__p_d, __p_s, 0, __del, __cookie);
    }

    ::Ice::AsyncResultPtr begin_op(::Ice::Double __p_d, const ::std::string& __p_s, const ::Ice::Context& __ctx, const ::Demo::Callback_MyInterface_opPtr& __del, const ::Ice::LocalObjectPtr& __cookie = 0)
    {
        return begin_op(__p_d, __p_s, &__ctx, __del, __cookie);
    }

    ::Ice::Int end_op(::Ice::Int& __p_i, ::Ice::Long& __p_l, const ::Ice::AsyncResultPtr&);
    
private:

    ::Ice::Int op(::Ice::Double, const ::std::string&, ::Ice::Int&, ::Ice::Long&, const ::Ice::Context*);
    ::Ice::AsyncResultPtr begin_op(::Ice::Double, const ::std::string&, const ::Ice::Context*, const ::IceInternal::CallbackBasePtr&, const ::Ice::LocalObjectPtr& __cookie = 0);
    
public:
    
    ::IceInternal::ProxyHandle<MyInterface> ice_context(const ::Ice::Context& __context) const
    {
        return dynamic_cast<MyInterface*>(::IceProxy::Ice::Object::ice_context(__context).get());
    }
    
    ::IceInternal::ProxyHandle<MyInterface> ice_adapterId(const ::std::string& __id) const
    {
        return dynamic_cast<MyInterface*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    }
    
    ::IceInternal::ProxyHandle<MyInterface> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
        return dynamic_cast<MyInterface*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    }
    
    ::IceInternal::ProxyHandle<MyInterface> ice_locatorCacheTimeout(int __timeout) const
    {
        return dynamic_cast<MyInterface*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    }
    
    ::IceInternal::ProxyHandle<MyInterface> ice_connectionCached(bool __cached) const
    {
        return dynamic_cast<MyInterface*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    }
    
    ::IceInternal::ProxyHandle<MyInterface> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
        return dynamic_cast<MyInterface*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    }
    
    ::IceInternal::ProxyHandle<MyInterface> ice_secure(bool __secure) const
    {
        return dynamic_cast<MyInterface*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    }
    
    ::IceInternal::ProxyHandle<MyInterface> ice_preferSecure(bool __preferSecure) const
    {
        return dynamic_cast<MyInterface*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    }
    
    ::IceInternal::ProxyHandle<MyInterface> ice_router(const ::Ice::RouterPrx& __router) const
    {
        return dynamic_cast<MyInterface*>(::IceProxy::Ice::Object::ice_router(__router).get());
    }
    
    ::IceInternal::ProxyHandle<MyInterface> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
        return dynamic_cast<MyInterface*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    }
    
    ::IceInternal::ProxyHandle<MyInterface> ice_collocationOptimized(bool __co) const
    {
        return dynamic_cast<MyInterface*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    }
    
    ::IceInternal::ProxyHandle<MyInterface> ice_invocationTimeout(int __timeout) const
    {
        return dynamic_cast<MyInterface*>(::IceProxy::Ice::Object::ice_invocationTimeout(__timeout).get());
    }
    
    ::IceInternal::ProxyHandle<MyInterface> ice_twoway() const
    {
        return dynamic_cast<MyInterface*>(::IceProxy::Ice::Object::ice_twoway().get());
    }
    
    ::IceInternal::ProxyHandle<MyInterface> ice_oneway() const
    {
        return dynamic_cast<MyInterface*>(::IceProxy::Ice::Object::ice_oneway().get());
    }
    
    ::IceInternal::ProxyHandle<MyInterface> ice_batchOneway() const
    {
        return dynamic_cast<MyInterface*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    }
    
    ::IceInternal::ProxyHandle<MyInterface> ice_datagram() const
    {
        return dynamic_cast<MyInterface*>(::IceProxy::Ice::Object::ice_datagram().get());
    }
    
    ::IceInternal::ProxyHandle<MyInterface> ice_batchDatagram() const
    {
        return dynamic_cast<MyInterface*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    }
    
    ::IceInternal::ProxyHandle<MyInterface> ice_compress(bool __compress) const
    {
        return dynamic_cast<MyInterface*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    }
    
    ::IceInternal::ProxyHandle<MyInterface> ice_timeout(int __timeout) const
    {
        return dynamic_cast<MyInterface*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    }
    
    ::IceInternal::ProxyHandle<MyInterface> ice_connectionId(const ::std::string& __id) const
    {
        return dynamic_cast<MyInterface*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
    }
    
    ::IceInternal::ProxyHandle<MyInterface> ice_encodingVersion(const ::Ice::EncodingVersion& __v) const
    {
        return dynamic_cast<MyInterface*>(::IceProxy::Ice::Object::ice_encodingVersion(__v).get());
    }
    
    static const ::std::string& ice_staticId();

private: 
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

}

}

namespace Demo
{

class MyInterface : virtual public ::Ice::Object
{
public:

    typedef MyInterfacePrx ProxyType;
    typedef MyInterfacePtr PointerType;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::Ice::Int op(::Ice::Double, const ::std::string&, ::Ice::Int&, ::Ice::Long&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___op(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

protected:
    virtual void __writeImpl(::IceInternal::BasicStream*) const;
    virtual void __readImpl(::IceInternal::BasicStream*);
    using ::Ice::Object::__writeImpl;
    using ::Ice::Object::__readImpl;
};

inline bool operator==(const MyInterface& l, const MyInterface& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

inline bool operator<(const MyInterface& l, const MyInterface& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

}

namespace Demo
{

template<class T>
class CallbackNC_MyInterface_op : public Callback_MyInterface_op_Base, public ::IceInternal::TwowayCallbackNC<T>
{
public:

    typedef IceUtil::Handle<T> TPtr;

    typedef void (T::*Exception)(const ::Ice::Exception&);
    typedef void (T::*Sent)(bool);
    typedef void (T::*Response)(::Ice::Int, ::Ice::Int, ::Ice::Long);

    CallbackNC_MyInterface_op(const TPtr& obj, Response cb, Exception excb, Sent sentcb)
        : ::IceInternal::TwowayCallbackNC<T>(obj, cb != 0, excb, sentcb), _response(cb)
    {
    }

    virtual void completed(const ::Ice::AsyncResultPtr& __result) const
    {
        ::Demo::MyInterfacePrx __proxy = ::Demo::MyInterfacePrx::uncheckedCast(__result->getProxy());
        ::Ice::Int i;
        ::Ice::Long l;
        ::Ice::Int __ret;
        try
        {
            __ret = __proxy->end_op(i, l, __result);
        }
        catch(const ::Ice::Exception& ex)
        {
            ::IceInternal::CallbackNC<T>::exception(__result, ex);
            return;
        }
        if(_response)
        {
            (::IceInternal::CallbackNC<T>::_callback.get()->*_response)(__ret, i, l);
        }
    }

    private:

    Response _response;
};

template<class T> Callback_MyInterface_opPtr
newCallback_MyInterface_op(const IceUtil::Handle<T>& instance, void (T::*cb)(::Ice::Int, ::Ice::Int, ::Ice::Long), void (T::*excb)(const ::Ice::Exception&), void (T::*sentcb)(bool) = 0)
{
    return new CallbackNC_MyInterface_op<T>(instance, cb, excb, sentcb);
}

template<class T> Callback_MyInterface_opPtr
newCallback_MyInterface_op(T* instance, void (T::*cb)(::Ice::Int, ::Ice::Int, ::Ice::Long), void (T::*excb)(const ::Ice::Exception&), void (T::*sentcb)(bool) = 0)
{
    return new CallbackNC_MyInterface_op<T>(instance, cb, excb, sentcb);
}

template<class T, typename CT>
class Callback_MyInterface_op : public Callback_MyInterface_op_Base, public ::IceInternal::TwowayCallback<T, CT>
{
public:

    typedef IceUtil::Handle<T> TPtr;

    typedef void (T::*Exception)(const ::Ice::Exception& , const CT&);
    typedef void (T::*Sent)(bool , const CT&);
    typedef void (T::*Response)(::Ice::Int, ::Ice::Int, ::Ice::Long, const CT&);

    Callback_MyInterface_op(const TPtr& obj, Response cb, Exception excb, Sent sentcb)
        : ::IceInternal::TwowayCallback<T, CT>(obj, cb != 0, excb, sentcb), _response(cb)
    {
    }

    virtual void completed(const ::Ice::AsyncResultPtr& __result) const
    {
        ::Demo::MyInterfacePrx __proxy = ::Demo::MyInterfacePrx::uncheckedCast(__result->getProxy());
        ::Ice::Int i;
        ::Ice::Long l;
        ::Ice::Int __ret;
        try
        {
            __ret = __proxy->end_op(i, l, __result);
        }
        catch(const ::Ice::Exception& ex)
        {
            ::IceInternal::Callback<T, CT>::exception(__result, ex);
            return;
        }
        if(_response)
        {
            (::IceInternal::Callback<T, CT>::_callback.get()->*_response)(__ret, i, l, CT::dynamicCast(__result->getCookie()));
        }
    }

    private:

    Response _response;
};

template<class T, typename CT> Callback_MyInterface_opPtr
newCallback_MyInterface_op(const IceUtil::Handle<T>& instance, void (T::*cb)(::Ice::Int, ::Ice::Int, ::Ice::Long, const CT&), void (T::*excb)(const ::Ice::Exception&, const CT&), void (T::*sentcb)(bool, const CT&) = 0)
{
    return new Callback_MyInterface_op<T, CT>(instance, cb, excb, sentcb);
}

template<class T, typename CT> Callback_MyInterface_opPtr
newCallback_MyInterface_op(T* instance, void (T::*cb)(::Ice::Int, ::Ice::Int, ::Ice::Long, const CT&), void (T::*excb)(const ::Ice::Exception&, const CT&), void (T::*sentcb)(bool, const CT&) = 0)
{
    return new Callback_MyInterface_op<T, CT>(instance, cb, excb, sentcb);
}

}

#include <IceUtil/PopDisableWarnings.h>
#endif
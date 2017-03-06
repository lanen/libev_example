/* Copyright 2017 evan */
#include <mk/event.h>

#pragma mark IoEventListener

namespace mk {

    IoEventListener::IoEventListener() {
    }

    IoEventListener::~IoEventListener() {
    }

    void IoEventListener::OnAccept() {
    }

    void IoEventListener::OnWrite() {
    }

    void IoEventListener::OnRead() {
    }

    void IoEventListener::OnClose() {
    }

}  // namespace mk

#pragma mark IoEventLoop

namespace mk {

    IoEventLoop::IoEventLoop(int _fd) : fd(_fd) {
    }

    IoEventLoop::~IoEventLoop() {
    }

    void IoEventLoop::Start() {
    }

}  // namespace mk

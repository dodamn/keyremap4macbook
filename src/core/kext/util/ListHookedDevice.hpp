#ifndef LISTHOOKEDDEVICE_HPP
#define LISTHOOKEDDEVICE_HPP

#include "base.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class HookedDevice {
    friend class ListHookedDevice;

  public:
    HookedDevice(void) : device_(NULL) {}

    IOHIDevice* get(void) const { return device_; }

  protected:
    IOHIDevice* device_;

    virtual bool initialize(IOHIDevice* d) = 0;
    virtual bool refresh(void) = 0;
    virtual bool terminate(void) = 0;

    static bool isIgnoreDevice(IOHIDevice* dev);
    static bool isConsumer(const char* name);
  };

  class ListHookedDevice {
  public:
    bool initialize(void);
    bool append(IOHIDevice* device);
    void terminate(void);
    bool terminate(const IOHIDevice* device);

    HookedDevice* get(const IOHIDevice* device);
    HookedDevice* get(void);
    void refresh(void);

  protected:
    enum {
      MAXNUM = 16,
    };
    ListHookedDevice(void) : last_(NULL), lock_(NULL) {}
    virtual ~ListHookedDevice(void) {}

  private:
    virtual HookedDevice* getItem(int index) = 0;
    HookedDevice* get_nolock(const IOHIDevice* device);

    const IOHIDevice* last_;
    IOLock* lock_;
  };
}

#endif

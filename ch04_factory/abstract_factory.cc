#include <iostream>
#include <string>
#include <vector>
#include <memory>
using namespace std;

enum class Version {High=1, Low};
enum class OS_Name {Android=1, IOS, WIN};

class IVirtualSystem {
public:
    virtual void boot() = 0;
    virtual void test() = 0;
    virtual void report() = 0;
};

class AndroidOS : public IVirtualSystem {
public:
    void boot() override { cout << "boot " << name << endl; };
    void test() override { cout << "test " << name << endl; };
    void report() override { cout << "report " << name << endl; };
private:
    const string name = "AndroidOS";
};

class GingerBread final : public AndroidOS {
public:
    void boot() override { cout << "boot " << name << endl; };
    void test() override { cout << "test " << name << endl; };
    void report() override { cout << "report " << name << endl; };
private:
    const string name = "GingerBread";
};

class Nougat final : public AndroidOS {
public:
    void boot() override { cout << "boot " << name << endl; };
    void test() override { cout << "test " << name << endl; };
    void report() override { cout << "report " << name << endl; };
private:
    const string name = "Nougat";
};

class IPhoneOS : public IVirtualSystem {
public:
    void boot() override { cout << "boot " << name << endl; };
    void test() override { cout << "test " << name << endl; };
    void report() override { cout << "report " << name << endl; };
private:
    const string name = "IPhoneOS";
};

class IPhoneOS_7 final : public IPhoneOS {
public:
    void boot() override { cout << "boot " << name << endl; };
    void test() override { cout << "test " << name << endl; };
    void report() override { cout << "report " << name << endl; };
private:
    const string name = "IPhoneOS_7";
};

class IPhoneOS_14 final : public IPhoneOS {
public:
    void boot() override { cout << "boot " << name << endl; };
    void test() override { cout << "test " << name << endl; };
    void report() override { cout << "report " << name << endl; };
private:
    const string name = "IPhoneOS_14";
};

class Windows : public IVirtualSystem {
public:
    void boot() override { cout << "boot " << name << endl; };
    void test() override { cout << "test " << name << endl; };
    void report() override { cout << "report " << name << endl; };
private:
    const string name = "Windows";
};

class Windows_98 final : public Windows {
public:
    void boot() override { cout << "boot " << name << endl; };
    void test() override { cout << "test " << name << endl; };
    void report() override { cout << "report " << name << endl; };
private:
    const string name = "Windows98";
};

class Windows_10 final : public Windows {
public:
    void boot() override { cout << "boot " << name << endl; };
    void test() override { cout << "test " << name << endl; };
    void report() override { cout << "report " << name << endl; };
private:
    const string name = "Windows10";
};

class IVersionFactory {
public:
    IVersionFactory() = default;
    virtual ~IVersionFactory() = default;
    virtual AndroidOS* createAndroid() = 0;
    virtual IPhoneOS* createIPhoneOS() = 0;
    virtual Windows* createWindows() = 0;
    virtual string getVersion() = 0;
};

class HighVersionSystem final : public IVersionFactory {
public:
    AndroidOS* createAndroid() override {
        cout << "Nougat created .. " << endl;
        return new Nougat();
    }
    IPhoneOS* createIPhoneOS() override {
        cout << "IPhoneOS_14 created .. " << endl;
        return new IPhoneOS_14();
    }
    Windows* createWindows() override {
        cout << "Windows10 created .. " << endl;
        return new Windows_10();
    }
    string getVersion() override {
        return "High Version";
    }
};

class LowVersionSystem final : public IVersionFactory {
public:
    AndroidOS* createAndroid() override {
        cout << "GingerBread created .. " << endl;
        return new GingerBread();
    }
    IPhoneOS* createIPhoneOS() override {
        cout << "IPhoneOS_7 created .. " << endl;
        return new IPhoneOS_7();
    }
    Windows* createWindows() override {
        cout << "Windows98 created .. " << endl;
        return new Windows_98();
    }
    string getVersion() override {
        return "Low Version";
    }
};

class IDeviceSimulatorFactory {
public:
    IDeviceSimulatorFactory() = default;
    virtual ~IDeviceSimulatorFactory() {
        delete version_factory_;
        version_factory_ = nullptr;
    };
    void runTest(OS_Name os_name) {
        IVirtualSystem* virtual_system = buildVirtualOS(os_name);
        if (virtual_system) {
            virtual_system->boot();
            virtual_system->test();
            virtual_system->report();
            delete virtual_system;
            cout << "finished .." << endl;
        }
    }
    virtual string getName() = 0;

protected:
    void createVersionFactory(Version ver) {
        if (version_factory_ == nullptr) {
            switch (ver) {
                case Version::High: {
                    version_factory_ = new HighVersionSystem();
                    break;
                }
                case Version::Low: {
                    version_factory_ = new LowVersionSystem();
                    break;
                }
                default: {
                    cout << "error handler need to be here .." << endl;
                }
            }
        }
    }
    IVersionFactory* getVersionFactory() const {
        return version_factory_;
    }
    virtual IVirtualSystem* buildVirtualOS(OS_Name os_name) = 0;

private:
    IVersionFactory* version_factory_ = nullptr;
};

class MobileSimulator final : public IDeviceSimulatorFactory {
public:
    MobileSimulator(Version ver) {
        createVersionFactory(ver);
    }
    ~MobileSimulator() = default;
    string getName() {
        string name = "Mobile";
        IVersionFactory* version_factory = getVersionFactory();
        if (version_factory) {
            name += " " + version_factory->getVersion();
        }
        return name;  
    }

private:
    IVirtualSystem* buildVirtualOS(OS_Name os_name) {
        IVersionFactory* version_factory = getVersionFactory();
        if (version_factory == nullptr) {
            cout << "should be create version factory .." << endl;
            return nullptr;
        }
        IVirtualSystem* virtual_system = nullptr;
        switch (os_name) {
            case OS_Name::Android: {
                virtual_system = version_factory->createAndroid();
                break;
            }
            case OS_Name::IOS: {
                virtual_system = version_factory->createIPhoneOS();
                break;
            }
            case OS_Name::WIN: {
                virtual_system = version_factory->createWindows();
                break;
            }
            default: {
                cout << "error handler need to be here .." << endl;
            }
        }
        return virtual_system;
    }
};

class EmbeddedSimulator final : public IDeviceSimulatorFactory {
public:
    EmbeddedSimulator(Version ver) {
        createVersionFactory(ver);
    }
    ~EmbeddedSimulator() = default;
    string getName() {
        string name = "Embedded";
        IVersionFactory* version_factory = getVersionFactory();
        if (version_factory) {
            name += " " + version_factory->getVersion();
        }
        return name;  
    }

private:
    IVirtualSystem* buildVirtualOS(OS_Name os_name) {
        IVersionFactory* version_factory = getVersionFactory();
        if (version_factory == nullptr) {
            cout << "should be create version factory .." << endl;
            return nullptr;
        }
        IVirtualSystem* virtual_system = nullptr;
        switch (os_name) {
            case OS_Name::Android: {
                virtual_system = version_factory->createAndroid();
                break;
            }
            case OS_Name::IOS: {
                virtual_system = version_factory->createIPhoneOS();
                break;
            }
            case OS_Name::WIN: {
                virtual_system = version_factory->createWindows();
                break;
            }
            default: {
                cout << "error handler need to be here .." << endl;
            }
        }
        return virtual_system;
    }
};

void TEST() {
    vector<shared_ptr<IDeviceSimulatorFactory>> simulators = {
        make_shared<MobileSimulator>(Version::High), 
        make_shared<MobileSimulator>(Version::Low), 
        make_shared<EmbeddedSimulator>(Version::High), 
        make_shared<EmbeddedSimulator>(Version::Low)};
    for (auto s: simulators) {
        cout << s->getName() << endl;
        s->runTest(OS_Name::Android);
        s->runTest(OS_Name::IOS);
        s->runTest(OS_Name::WIN);
        cout << endl;
    }
}

int main() {
    TEST();
    return 0;
}
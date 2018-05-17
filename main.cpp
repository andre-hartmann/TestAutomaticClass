#include <iostream>
#include <vector>

using namespace std;

class KUnitTest;

typedef std::vector<KUnitTest *> UnitTests;

class KUnitTest
{
public:
    KUnitTest(const char *name);
    virtual ~KUnitTest();

    void run();
    std::string name() const;

    virtual void runTestImpl() = 0; // Abstract function for test implementation in derived classes

private:
    const std::string m_name;
};

static UnitTests *g_allTests = nullptr;

static void registerUnitTest(KUnitTest *unitTest)
{
    if (!g_allTests )
        g_allTests = new UnitTests();

    g_allTests->push_back(unitTest);
}

void runAllTests(void)
{
    if (!g_allTests) {
        std::cerr << "No tests registered." << endl;
        return;
    }

    for (auto test : *g_allTests)
        test->run();
}

KUnitTest::KUnitTest(const char *name)  :
    m_name(name)
{
    registerUnitTest(this);
}

KUnitTest::~KUnitTest()
{
}

void KUnitTest::run()
{
    std::cout << name() << ": start" << endl;

    runTestImpl();

    std::cout << name() << ": end" << endl;
}

string KUnitTest::name() const
{
    return m_name;
}

#define TEST(x) \
    class KUnitTest##x : public KUnitTest { \
    public: \
        KUnitTest##x() : KUnitTest(#x) { } \
        void runTestImpl() override; \
    }; \
    static KUnitTest##x x; \
    void KUnitTest##x::runTestImpl()

TEST(Hello)
{
    std::cout << "In Test: '" << name() << "'" << endl;
}

TEST(MyWorld)
{
    std::cout << "In Test: '" << name() << "'" << endl;
}

int main(int, char *[])
{
    runAllTests();
    return 0;
}

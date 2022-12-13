#include <bits/stdc++.h>
#include <list>
#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>
#include <netinet/in.h>
#include "msg_server.h"

using namespace CppUnit;
using namespace std;

//-----------------------------------------------------------------------------

class TestTcpServer : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestTcpServer);
    CPPUNIT_TEST(Testinit_server);
    CPPUNIT_TEST(start_server);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp(void);
    void tearDown(void);

protected:
    
    void Testinit_server(void);
    void start_server(void);
    

private:

    TcpServer *msg_server;
};

//-----------------------------------------------------------------------------


void
TestTcpServer::Teststart_server(void)
{
    CPPUNIT_ASSERT(0 == msg_server->start_server());
}
void
TestTcpServer::Testinit_server(void)
{
    CPPUNIT_ASSERT(0 == msg_server->init_server());
}

void TestTcpServer::setUp(void)
{
msg_server = new TcpServer();


}

void TestTcpServer::tearDown(void)
{
delete msg_server;
	}

CPPUNIT_TEST_SUITE_REGISTRATION( TestTcpServer);



//-----------------------------------------------------------------------------


int main(int argc, char* argv[])
{
    // informs test-listener about testresults
    CPPUNIT_NS::TestResult testresult;

    // register listener for collecting the test-results
    CPPUNIT_NS::TestResultCollector collectedresults;
    testresult.addListener (&collectedresults);

    // register listener for per-test progress output
    CPPUNIT_NS::BriefTestProgressListener progress;
    testresult.addListener (&progress);

    // insert test-suite at test-runner by registry
    CPPUNIT_NS::TestRunner testrunner;
    testrunner.addTest (CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest ());
    testrunner.run(testresult);

    // output results in compiler-format
    CPPUNIT_NS::CompilerOutputter compileroutputter(&collectedresults, std::cerr);
    compileroutputter.write ();

    // Output XML for Jenkins CPPunit plugin
    ofstream xmlFileOut("Testserver.xml");
    XmlOutputter xmlOut(&collectedresults, xmlFileOut);
    xmlOut.write();

    // return 0 if tests were successful
    return collectedresults.wasSuccessful() ? 0 : 1;
}


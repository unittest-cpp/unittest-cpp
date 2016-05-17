#include "TestList.h"
#include "Test.h"

#include <cassert>

namespace UnitTest {

	TestListNode::TestListNode(Test* test)
		: m_test(test),
		m_next(0)
	{
	}

   TestList::TestList()
      : m_head(0)
      , m_tail(0)
   {}

   void TestList::Add(Test* test)
   {
      if (m_tail == 0)
      {
         assert(m_head == 0);
         m_head = test;
         m_tail = test;
      }
      else
      {
         m_tail->m_nextTest = test;
         m_tail = test;
      }
   }

   Test* TestList::GetHead() const
   {
      return m_head;
   }

   ListAdder::ListAdder(TestList& list, Test* test)
   {
      list.Add(test);
   }

}

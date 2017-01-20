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

   TestListNode* TestList::Add(Test* test)
   {
      TestListNode* node = 0;
      if (m_tail == 0)
      {
         assert(m_head == 0);
         node = new TestListNode(test);
         m_head = node;
         m_tail = node;
      }
      else
      {
         node = new TestListNode(test);
         m_tail->m_next = node;
         m_tail = node;
      }
      return node;
   }

   TestListNode* TestList::GetHead() const
   {
      return m_head;
   }

   ListAdder::ListAdder(TestList& list, Test* test)
   {
      list.Add(test);
   }

}

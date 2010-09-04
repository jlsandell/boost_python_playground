from embiggenator import embiggenator
import unittest

TIMEIT_TEST = False

class embiggenatorTest(unittest.TestCase):
    EXAMPLE_TEXT = 'foo bar baz'
    text = None

    def setUp(self):
        self.text = embiggenator(self.EXAMPLE_TEXT)

    def testinit(self):
        self.assertEqual(str(self.text), self.EXAMPLE_TEXT.upper())

    def testSet(self):
        newtext = 'baz bar foo'
        self.text.set(newtext)
        self.assertEqual(str(self.text), newtext.upper())

    def testGet(self):
        self.assertEqual(self.text.get(), self.EXAMPLE_TEXT.upper())
    
    
if __name__ == '__main__':
    unittest.main()
    
    if TIMEIT_TEST:
        import timeit

        EXAMPLE_TEXT = 'foo bar baz'

        def one(text):
            example = embiggenator(text)
            return example

        def two(text):
            example = text.upper()
            return example
        

        t = timeit.Timer(setup='from __main__ import one, EXAMPLE_TEXT', stmt='one(EXAMPLE_TEXT)')
        t2 = timeit.Timer(setup='from __main__ import two, EXAMPLE_TEXT', stmt='two(EXAMPLE_TEXT)')

        for i in xrange(0,10):
            print '%-15s %s' % ('embiggenator:', t.timeit(42))
            print '%-15s %s' % ('upper:', t2.timeit(42))
            print

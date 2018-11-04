#ifdef _QSORT_SPECIALIZED_TYPE
// Specialized implementation, without obj_size argument.
#define _PARAMETER_OBJ_SIZE
#define _ARGUMENT_OBJ_SIZE

// The obj_size is a static value, for compilers to optimize.
#define obj_size _QSORT_SPECIALIZED_TYPE_SIZE
#else
// General implementation, with obj_size argument.
#define _PARAMETER_OBJ_SIZE size_t obj_size, // Used in function definition.
#define _ARGUMENT_OBJ_SIZE  obj_size,        // Used in function calling.
#endif

static inline void _swap(_PARAMETER_OBJ_SIZE
                         char *a, char *b) {
    if (a == b) return;

    char temp[obj_size];
    _specialized_memcpy(temp, a, obj_size);
    _specialized_memcpy(a, b, obj_size);
    _specialized_memcpy(b, temp, obj_size);
}

static inline void _insertion_sort(char *array,
                                   size_t n,
                                   _PARAMETER_OBJ_SIZE
                                   compare_func_t compare_func) {
    for (char *p = array + obj_size; p < array + obj_size * n; p += obj_size) {
        char temp[obj_size];
        // In many cases the next loop won't execute, so in order to prevent
        // useless memory copy, backup current element it in the first cycle.

        char *q = p - obj_size;
        while (q >= array && compare_func(q + obj_size == p ? p : temp, q) < 0) {
            // Backup current element.
            if (q + obj_size == p) _specialized_memcpy(temp, p, obj_size);

            // Right-shift the elements.
            _specialized_memcpy(q + obj_size, q, obj_size);
            q -= obj_size;
        }

        // Insert the current element.
        if (q + obj_size != p) _specialized_memcpy(q + obj_size, temp, obj_size);
    }
}

static inline void _heap_sort_heapify(char *array,
                                      size_t i,
                                      size_t n,
                                      _PARAMETER_OBJ_SIZE
                                      compare_func_t compare_func) {
    while (i * 2 + 1 < n) {
        size_t lc = i * 2 + 1,
               rc = i * 2 + 2,
               larger_child;

        larger_child = (rc >= n || compare_func(array + lc * obj_size,
                                                array + rc * obj_size) > 0) ? lc : rc;
        
        if (compare_func(array + larger_child * obj_size, array + i * obj_size) > 0) {
            _swap(_ARGUMENT_OBJ_SIZE array + larger_child * obj_size, array + i * obj_size);
            i = larger_child;
        } else break;
    }
}

static inline void _heap_sort(char *array,
                              size_t n,
                              _PARAMETER_OBJ_SIZE
                              compare_func_t compare_func) {
    // Make a heap first.
    // Do heapify from the largest non-leaf node.
    for (size_t i = (n - 1) / 2; i < n; i--) {
        _heap_sort_heapify(array, i, n, _ARGUMENT_OBJ_SIZE compare_func);
    }

    // Pop elements from heap and place them from the back to the front of the array.
    for (size_t m = n; m > 1; m--) {
        // Swap a[0] (top of heap) with last element in heap.
        _swap(_ARGUMENT_OBJ_SIZE array + 0 * obj_size, array + (m - 1) * obj_size);

        // Maintain the heap
        _heap_sort_heapify(array, 0, m - 1, _ARGUMENT_OBJ_SIZE compare_func);
    }
}

// Return the median of the first, last, middle element.
static inline char *_quick_sort_choose_pivot(char *array,
                                             size_t n,
                                             _PARAMETER_OBJ_SIZE
                                             compare_func_t compare_func) {
    char *p1 = array + (0)     * obj_size, // first
         *p2 = array + (n / 2) * obj_size, // middle
         *p3 = array + (n - 1) * obj_size; // last

    return compare_func(p1, p2) < 0
           // p1 < p2 && p2 < p3
           ? (compare_func(p2, p3) < 0 ? p2 : (compare_func(p1, p3) < 0 ? p3 : p1))
                                              // p1 < p2 && p3 <= p2
           // p1 >= p2 && p2 >= p3
           : (compare_func(p2, p3) > 0 ? p2 : (compare_func(p1, p3) > 0 ? p3 : p1));
                                              // p1 >= p2 && p3 > p2
}

// Partition the array into two parts in O(n) time. The left part's are all <= pivot 
// and the right part's are all >= pivot.
static inline void _quick_sort_partition(char *array,
                                         size_t n,
                                         _PARAMETER_OBJ_SIZE
                                         compare_func_t compare_func,
                                         char *pivot,
                                         char **p_pivot_l,
                                         char **p_pivot_r) {
    // Put pivot to the first of the array.
    _swap(_ARGUMENT_OBJ_SIZE array + 0 * obj_size, pivot);
    pivot = array + 0 * obj_size;

    // Init with l = 1, r = n. l++ and r-- will be executed firstly.
    char *l = array + obj_size * 0, *r = array + obj_size * n;
    while (1) {
        // Find the first (or next) element that >= pivot.
        do
            l += obj_size;
        while (compare_func(l, pivot) < 0);

        // Find the last (or previous) element that <= pivot.
        do
            r -= obj_size;
        while (compare_func(r, pivot) > 0);

        // Now r <= pivot, [1, l - 1] <= pivot, r > l - 1, so we can put pivot to r
        // and put r to first of the array.
        if (l >= r) {
            _swap(_ARGUMENT_OBJ_SIZE pivot, r);

            // Find left-most and right-most element that equals to pivot.
            char *pivot_l = r, *pivot_r = r;

            while (pivot_l != array && !compare_func(pivot_l, pivot_l - obj_size))
                pivot_l -= obj_size;
            while (pivot_r != array + (n - 1) * obj_size && !compare_func(pivot_r, pivot_r + obj_size))
                pivot_r += obj_size;

            // Return them.
            *p_pivot_l = pivot_l;
            *p_pivot_r = pivot_r;

            return;
        }

        // Swap them to make sure no elements > pivot in the left part and no
        // elements > pivot in right part.
        _swap(_ARGUMENT_OBJ_SIZE l, r);
    }
}

void _quick_sort(char *array,
                 size_t n,
                 _PARAMETER_OBJ_SIZE
                 compare_func_t compare_func,
                 size_t max_depth) {
    // Use a custom stack to prevent recursive.
    // max stack size = 2 * log2(max n)
    const size_t STACK_SIZE = CHAR_BIT * sizeof(size_t) * 2;

    struct {
        char *array;
        size_t n;
    } stack_bottom[STACK_SIZE], *stack_top = stack_bottom - 1;

#define _stack_push(_array, _n) ((stack_top++), (stack_top->array = _array), (stack_top->n = _n))
#define _stack_pop(_array, _n)  ((_array = stack_top->array), (_n = stack_top->n), (stack_top--))
#define _stack_size()           (size_t)(stack_top - (stack_bottom - 1))

#define _next_state_from_stack() { \
        if (_stack_size() > 0) {   \
            _stack_pop(array, n);  \
            continue;              \
        } else return;             \
    }

    while (1) {
        // If there're few elements, use insertion sort instead.
        const int INSERTION_SORT_COUNT_LIMIT = 7;
        if (n <= INSERTION_SORT_COUNT_LIMIT) {
            _insertion_sort(array, n, _ARGUMENT_OBJ_SIZE compare_func);
            _next_state_from_stack();
        }

        char *pivot = _quick_sort_choose_pivot(array, n, _ARGUMENT_OBJ_SIZE compare_func);

        char *pivot_l, *pivot_r;
        _quick_sort_partition(array, n, _ARGUMENT_OBJ_SIZE compare_func, pivot,
                              &pivot_l, &pivot_r);

        // Sort the two parts.
        size_t left_n = (pivot_l - array) / obj_size,
               right_n = n - (pivot_r - array) / obj_size - 1;
        char *array_l = array,
             *array_r = pivot_r + obj_size;

        // In the code below we perfer to recursive right part, so make sure left
        // part have more elements.
        if (left_n < right_n) {
            size_t temp_n = left_n;
            left_n = right_n;
            right_n = temp_n;

            char *array_temp = array_l;
            array_l = array_r;
            array_r = array_temp;
        }

        // Max recursive depth exceeded, use heap sort instead.
        if (_stack_size() == max_depth) {
            if (left_n > 1) _heap_sort(array_l, left_n, _ARGUMENT_OBJ_SIZE compare_func);
            if (right_n > 1) _heap_sort(array_r, right_n, _ARGUMENT_OBJ_SIZE compare_func);
            _next_state_from_stack();
        }

        // Continue to use quick sort.
        // If the left part (which have more elements) have more than one element, sort it.
        if (left_n > 1) {
            // Do we need to sort the right part?
            if (right_n > INSERTION_SORT_COUNT_LIMIT)
                _stack_push(array_r, right_n);
            else if (right_n > 1)
                _insertion_sort(array_r, right_n, _ARGUMENT_OBJ_SIZE compare_func);

            // Replace the arguments so in the next loop of this function it will sort the
            // left part.
            array = array_l;
            n = left_n;
            max_depth--;
        }
        // Oh, everything is sorted.
        else _next_state_from_stack();
    }
#undef _next_state_from_stack

#undef _stack_push
#undef _stack_pop
#undef _stack_size
}

void _qsort(void *base,
            size_t n,
            _PARAMETER_OBJ_SIZE
            compare_func_t compare_func) {
    // Calculate log2(n).
    size_t log2n = 0;
    while (((size_t)1 << (log2n + 1)) <= n) log2n++;

    // If recursive depth > MAX_DEPTH_TIMES_OF_LOG2N * log2(n), use
    // heap sort instead.
    const int MAX_DEPTH_TIMES_OF_LOG2N = 2;
    size_t max_depth = log2n * MAX_DEPTH_TIMES_OF_LOG2N;

    _quick_sort(base, n, _ARGUMENT_OBJ_SIZE compare_func, max_depth);
}

#undef _PARAMETER_OBJ_SIZE
#undef _ARGUMENT_OBJ_SIZE

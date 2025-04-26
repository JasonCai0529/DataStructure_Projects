/**
 * @file quackfun.cpp
 * This is where you will implement the required functions for the
 * stacks and queues portion of the lab.
 */

namespace QuackFun {

/**
 * Sums items in a stack.
 *
 * **Hint**: think recursively!
 *
 * @note You may modify the stack as long as you restore it to its original
 * values.
 *
 * @note You may use only two local variables of type T in your function.
 * Note that this function is templatized on the stack's type, so stacks of
 * objects overloading the + operator can be summed.
 *
 * @note We are using the Standard Template Library (STL) stack in this
 * problem. Its pop function works a bit differently from the stack we
 * built. Try searching for "stl stack" to learn how to use it.
 *
 * @param s A stack holding values to sum.
 * @return  The sum of all the elements in the stack, leaving the original
 *          stack in the same state (unchanged).
 */
template <typename T>
T sum(stack<T>& s)
{

    // Your code here

    // has to pop
    if (s.empty()) {
        return T();
    }
    T current = s.top();
    s.pop();
    T sum = current + QuackFun::sum(s);
    s.push(current);
    // pop and sum
    // add values back in
    return sum; // stub return value (0 for primitive types). Change this!
                // Note: T() is the default value for objects, and 0 for
                // primitive types
}

/**
 * Checks whether the given string (stored in a queue) has balanced brackets.
 * A string will consist of parentheses, curly/square brackets and other
 * characters. This function will return true if and only if the bracket
 * characters in the given string are balanced. For this to be true, all
 * brackets must be matched up correctly, with no extra, hanging, or unmatched
 * brackets. For example, the string "[hello=](){}" is balanced, "{()[{a}]}" is
 * balanced, "[})" is unbalanced, "][" is unbalanced, and "))))[cs225]" is
 * unbalanced.
 *
 * For this function, you may only create a single local variable of type
 * `stack<char>`! No other stack or queue local objects may be declared. Note
 * that you may still declare and use other local variables of primitive types.
 *
 * @param input The queue representation of a string to check for balanced brackets in
 * @return      Whether the input string had balanced brackets
 */
bool isBalanced(queue<char> input)
{
    // @TODO: Make less optimistic
    stack<char> s;

    while (!input.empty()) {
        char cur = input.front();
        // only push open bracket inside stack
        if (cur == '{' || cur == '[' || cur == '(') {
            s.push(cur);
        } else if (cur == '}' || cur == ']' || cur == ')') {
            if (s.empty()) {
                return false;
            } else if (cur == ']' && s.top() != '[') {
                return false;
            } else if (cur == ')' && s.top() != '(') {
                return false;
            } else if (cur == '}' && s.top() != '{') {
                return false;
            }
            s.pop();
        }
        input.pop();
    }
    // stack should be empty in the end
    return s.empty();
}

/**
 * Reverses even sized blocks of items in the queue. Blocks start at size
 * one and increase for each subsequent block.
 *
 * **Hint**: You'll want to make a local stack variable.
 *
 * @note Any "leftover" numbers should be handled as if their block was
 * complete.
 *
 * @note We are using the Standard Template Library (STL) queue in this
 * problem. Its pop function works a bit differently from the stack we
 * built. Try searching for "stl stack" to learn how to use it.
 *
 * @param q A queue of items to be scrambled
 */
template <typename T>
void scramble(queue<T>& q)
{
    stack<T> s;
    queue<T> copy_q;

    while (!q.empty()) {
        T cur = q.front();
        q.pop();
        copy_q.push(cur);
    }

    bool r = false;
    unsigned int amount = 2;
    unsigned int idx = 1;
    while (!copy_q.empty()) {
        // loop for not rotation
        if (!r) {
            for (unsigned int i = 0; i < idx; i++) {
                if (copy_q.empty()) {break;}
                q.push(copy_q.front());
                copy_q.pop();
            }
            idx += 2;
            r = true;
        } else if (r) {
            for (unsigned int i = 0; i < amount; i++) {
                if (copy_q.empty()) {break;}
                T cur = copy_q.front();
                copy_q.pop();
                s.push(cur);
            }

            for (unsigned int i = 0; i < amount; i++) {
                if (s.empty()) {break;}
                T s_cur = s.top();
                q.push(s_cur);
                s.pop();
            }
            r = false;
            amount += 2;
        }
    }
}


}

#pragma once
#include <vector>
#include <stack>

using std::vector;

template<class T>
class GameStack {
  public:

    /**
     * Default constructor.
     */
    GameStack() = default;

    /**
     * Default destructor.
     */
    ~GameStack()                         = default;

    /**
     * Deleted copy constructor.
     * @param other
     */
    GameStack(const GameStack<T> &other) = delete;

    /**
     * Deleted move constructor.
     * @param other
     */
    GameStack(GameStack<T> &&other) = delete;

    /**
     * Deleted copy assignment.
     * @param other
     * @return
     */
    GameStack<T> &operator=(const GameStack<T>& other) = delete;

    /**
     * Deleted move assignment.
     * @param other
     * @return
     */
    GameStack<T> &operator=(GameStack<T>&& other) = delete;

    /**
     * Add a new state onto the stack.
     * @param new_state the state to be added onto the stack.
     */
    void AddToStack(T new_state) {
        game_stack.push(new_state);
    }

    /**
     * Returns the top of the stacks state.
     * @return the top element
     */
    T& getTop() {
        return game_stack.top();
    }
    /**
     * Deletes the top element
     */
    void popTop() {
        remove_top_flag = true;
    }

    void checkTop() {
        if (remove_top_flag) {
            removeTop();
        }
    }

    bool isRemoveTopFlag() const {
        return remove_top_flag;
    }

private:
    bool remove_top_flag = false;
    size_t size = 0;
    /// The stack itself.
    std::stack<T> game_stack;

    /*
     * Finds an element on the stack.
     */
    void removeTop() {
        game_stack.pop();
        remove_top_flag = false;
    }
};

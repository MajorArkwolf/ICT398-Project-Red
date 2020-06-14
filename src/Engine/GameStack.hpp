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
     * @param newState the state to be added onto the stack.
     */
    void AddToStack(T newState) {
        gameStack.push(newState);
    }

    /**
     * Returns the top of the stacks state.
     * @return
     */
    T& getTop() {
        return gameStack.top();
    }
    /**
     * Deletes the top element
     */
    void popTop() {
        removeTopFlag = true;
    }

    void checkTop() {
        if (removeTopFlag) {
            removeTop();
        }
    }

    bool isRemoveTopFlag() const {
        return removeTopFlag;
    }

private:
    bool removeTopFlag = false;
    size_t size = 0;
    /// The stack itself.
    std::stack<T> gameStack;

    /*
     * Finds an element on the stack.
     */
    void removeTop() {
        gameStack.pop();
        removeTopFlag = false;
    }
};

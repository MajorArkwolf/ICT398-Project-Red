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
        game_stack_.push(new_state);
    }

    /**
     * Returns the top of the stacks state.
     * @return the top element
     */
    T& getTop() {
        return game_stack_.top();
    }
    /**
     * Get ready to pop the top.
     */
    void popTop() {
        remove_top_flag_ = true;
    }

    /**
     * Checks if the top is ready to pop and if so pop up.
     */
    void checkTop() {
        if (remove_top_flag_) {
            removeTop();
        }
    }

    /**
     * Checks to see if the stack wants to deallocate something.
     * @return true if ready.
     */
    bool isRemoveTopFlag() const {
        return remove_top_flag_;
    }

    /**
     * Checks to see if deallocating will cause their to be no scene.
     * @return true if 1 or less is on the stack.
     */
    bool WillBeEmpty() {
        return game_stack_.size() <= 1;
    }

    /**
     * Checks to see if the stack is empty.
     * @return true if empty
     */
    bool Empty() {
        return game_stack_.empty();
    }

    void Clear() {
        while (!game_stack_.empty()) {
            game_stack_.pop();
        }
    }

private:
    bool remove_top_flag_ = false;
    /// The stack itself.
    std::stack<T> game_stack_;

    /*
     * Removes the top when its safe to do so.
     */
    void removeTop() {
        game_stack_.pop();
        remove_top_flag_ = false;
    }
};

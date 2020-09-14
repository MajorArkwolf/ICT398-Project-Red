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
     * Deletes the top element
     */
    void popTop() {
        remove_top_flag_ = true;
    }

    void checkTop() {
        if (remove_top_flag_) {
            removeTop();
        }
    }

    bool isRemoveTopFlag() const {
        return remove_top_flag_;
    }

    bool WillBeEmpty() {
        return game_stack_.size() <= 1;
    }

    bool Empty() {
        return game_stack_.size() == 0;
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
     * Finds an element on the stack.
     */
    void removeTop() {
        game_stack_.pop();
        remove_top_flag_ = false;
    }
};

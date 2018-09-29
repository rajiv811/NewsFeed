/**
 * @file NewsFeed.cpp - Implementation of NewsFeed.h
 * @author Rajiv Singireddy
 * @see "Seattle University, CPSC2430, Spring 2018"
 */

#include "NewsFeed.h"
using namespace std;


void NewsFeed::bubble(size_t child) {
    while (!is_root(child) && value(child) < value(parent(child))) {
      size_t p = parent(child);
      swap(child, p);
      child = p;
    }
}

void NewsFeed::percolate(size_t p) {
    while (!is_leaf(p)) {
      size_t child = left(p);
      if (has_right(p) && value(right(p)) < value(child))
        child = right(p);
      if (value(p) <= value(child))
        return;
      swap(child, p);
      p = child;
    }
}
 
NewsFeed::Staleness NewsFeed::value(size_t i) const {
    return weight(heap[i]);
}

void NewsFeed::swap(size_t i, size_t j) {
    std::swap(heap[i], heap[j]);
    locations.add(heap[i], i);
    locations.add(heap[j], j);
}

bool NewsFeed::valid(size_t i) const {
    return i <= n && i >= root();
}

bool NewsFeed::is_root(size_t i) const {
    return i == root();
}

bool NewsFeed::is_leaf(size_t i) const {
    return !valid(left(i));
}

bool NewsFeed::has_right(size_t p) const {
    return valid(right(p));
}

void NewsFeed::resize() {
    NewsFeed::Headline *oldheap = heap;
    capacity = 2*capacity + 17;
    heap = new NewsFeed::Headline[capacity+1]; // the +1 is b/c we don't use heap[0]
    for (size_t i = 1; i <= n; i++)
      heap[i] = oldheap[i];
    delete[] oldheap;
}

size_t NewsFeed::root() {
    return 1;
}

size_t NewsFeed::parent(size_t child) {
    return child/2;
}

size_t NewsFeed::left(size_t p) {
    return p*2;
}

size_t NewsFeed::right(size_t p) {
    return left(p)+1;
}

NewsFeed::NewsFeed() : heap(nullptr), n(0), capacity(0) {}

NewsFeed::~NewsFeed() {
    delete[] heap;
}

NewsFeed::NewsFeed(const NewsFeed &other) = delete;
NewsFeed::NewsFeed(NewsFeed &temp) = delete;
NewsFeed& NewsFeed::operator =(const NewsFeed &other) = delete;
NewsFeed& NewsFeed::operator =(NewsFeed &&temp) = delete;

void NewsFeed::enqueue(NewsFeed::Headline headline, NewsFeed::Story story, NewsFeed::Staleness weight) {
    if (n == capacity)
      resize();
    stories.add(headline, story);
    staleness.add(headline, weight);
    locations.add(headline,n);
    heap[++n] = headline;
    bubble(n);
}

NewsFeed::Headline NewsFeed::peek() const {
    if (empty())
      throw std::invalid_argument("peek in empty heap");
    return heap[root()];
}

void NewsFeed::dequeue() {
    if (empty())
      throw std::invalid_argument("dequeue from empty heap");
    swap(root(), n--);
    percolate(root());
}
bool NewsFeed::empty() const {
    return n == 0;
}

NewsFeed::Staleness NewsFeed::weight(NewsFeed::Headline headline) const {
    return staleness.get(headline);
}

void NewsFeed::reweight(NewsFeed::Headline headline, NewsFeed::Staleness newWeight) {
    int old = staleness.get(headline);
    staleness.add(headline, newWeight);

    const auto &map = locations;
    size_t loc = map.get(headline);

    if(newWeight > old)
      percolate(loc);
    else
      bubble(loc);
    size_t i = 1;
    while(heap[i]!=headline)
      i++;
    locations.add(headline,i);
}

NewsFeed::Story NewsFeed::get(NewsFeed::Headline headline) const {
    const auto &theHeap = stories;  // force using the const version of the DictHash::get() method
    return theHeap.get(headline);
}


NewsFeed::const_iterator NewsFeed::begin() const {
    return const_iterator(&stories, 0);
}

NewsFeed::const_iterator NewsFeed::end() const {
    return const_iterator(&stories, n);
}

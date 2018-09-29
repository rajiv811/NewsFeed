/**
 * @file NewsFeed.h - NewsFeed file implementing a PQ for p5
 * @author Rajiv Singireddy
 * @see "Seattle University, CPSC2430, Spring 2018"
 */

#include <iostream>
#include "DictHash.h"
#include <string>
#include "adt/PriorityQueue.h"
#pragma once

class NewsFeed {
 public:
  typedef std::string Headline;
  typedef std::string Story;
  typedef int Staleness;
  typedef std::hash<std::string> HeadlineHasher;
  typedef DictHash<Headline,Story,HeadlineHasher>::const_iterator const_iterator;

  NewsFeed();
  ~NewsFeed();
  NewsFeed(const NewsFeed &other);
  NewsFeed(NewsFeed &temp);
  NewsFeed& operator =(const NewsFeed &other);
  NewsFeed& operator =(NewsFeed &&temp);
  void enqueue(Headline headline, Story story, Staleness staleness);
  Headline peek() const;
  void dequeue();
  bool empty() const;
  Staleness weight(Headline headline) const;
  void reweight(Headline headline, Staleness staleness);
  Story get(Headline headline) const;
  const_iterator begin() const;
  const_iterator end() const;
  
 private:
  typedef size_t HeapLocation;

  void bubble(size_t child);
  void percolate(size_t p);
  Staleness value(size_t i) const;
  void swap(size_t i, size_t j);
  bool valid(size_t i) const;
  bool is_root(size_t i) const;
  bool is_leaf(size_t i) const;
  bool has_right(size_t p) const;
  void resize();
  
  static size_t root();
  static size_t parent(size_t child);
  static size_t left(size_t p);
  static size_t right(size_t p);

  DictHash<Headline, Story, HeadlineHasher> stories;
  DictHash<Headline, Staleness, HeadlineHasher> staleness;
  DictHash<Headline, HeapLocation, HeadlineHasher> locations;
  Headline *heap;
  size_t n;
  size_t capacity;
};

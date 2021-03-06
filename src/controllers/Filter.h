#ifndef FILTER_H_INCLUDED
#define FILTER_H_INCLUDED
#include <list>
#include <string>
#include <set>
#include <unordered_set>
#include "../models/Containers.hpp"
class Graph;
class Filter;


class FilterSet{
    private:
    std::list<Filter*> filters;

public:
    void addNewFilter(Filter* filter);
    void clearAllFilters();
    void clearFilter(Filter* filter);

    void processAll(Graph* graph);
    Graph* processAll(Graph* graph, int returnCopy);

};

class Filter{
protected:
    friend class FilterSet;
    virtual void process(Graph* graph)=0;
public:
	virtual unordered_set<Containers::Mail*> findMails(Graph* graph) = 0;
};


//zostawia tylko te mejle, ktore zawieraja w temacie okreslony podciag
class TopicSubstringFilter:public Filter{
    std::string substring;
    void process(Graph* graph);
public:
    TopicSubstringFilter(std::string substring);

	unordered_set<Containers::Mail*> findMails(Graph* graph);
};

class DateFilter:public Filter{
    bool before;
    unsigned int timeStamp;
    void process(Graph* graph);
public:
    DateFilter(unsigned int timeStamp, bool before); //jesli before==true to ma odrzucic wszystkie mejle PO dacie
                                                    //w przeciwnym wypadku odrzuca wszystkie mejle PRZED data
	unordered_set<Containers::Mail*> findMails(Graph* graph);
};


class PeopleFilter:public Filter{
    bool removeMailsFromSender;
    Containers::Person* person;
    void process(Graph* graph);
public:
    //jesli removeMailsFromSender==true to usunie mejl, ktorego nadawca jest wylistowany w secie
    //w przeciwnym wypadku usunie kazdy mejl odebrany przez wylisowane osoby
    PeopleFilter(Containers::Person* person, bool removeMailsFromSender);
	
	unordered_set<Containers::Mail*> findMails(Graph* graph);
};

class MailsFilter: public Filter
{
	std::unordered_set<Containers::Mail*> mails;
	void process(Graph* graph);
public:
	MailsFilter(std::unordered_set<Containers::Mail*> mails);
	
	unordered_set<Containers::Mail*> findMails(Graph* graph);
};

#endif // FILTER_H_INCLUDED

#ifndef SEARCHCONTROLLER_H
#define SEARCHCONTROLLER_H

#include "interface/Panel.h"
#include "SearchModel.h"
#include "SearchView.h"
class SearchView;
class SearchModel;
class SearchController
{
private:
	SearchModel * searchModel;
	SearchView * searchView;
public:
	SearchController();
	SearchView * GetView() { return searchView; }
	void DoSearch(std::string query);
};

#endif // SEARCHCONTROLLER_H
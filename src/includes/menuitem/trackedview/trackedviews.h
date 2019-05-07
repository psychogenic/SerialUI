/*
 * trackedviews.h
 *
 *  Created on: Sep 27, 2018
 *      Author: Pat Deegan
 *
 *  trackedviews is part of the SerialUI project.
 *  Copyright (C) 2018-2019 Pat Deegan, psychogenic.com
 */

#ifndef SERIALUI_SRC_INCLUDES_MENUITEM_TRACKEDVIEW_TRACKEDVIEWS_H_
#define SERIALUI_SRC_INCLUDES_MENUITEM_TRACKEDVIEW_TRACKEDVIEWS_H_

#include "../../SerialUITypes.h"
#include "../ItemTrackedView.h"


namespace SerialUI {
namespace Menu {
namespace Item {
namespace View {

typedef TrackedViewImpl<SerialUI::Tracked::View::ChartBar> BarChart;
typedef TrackedViewImpl<SerialUI::Tracked::View::ChartLineBasic> LineBasic;
typedef TrackedViewImpl<SerialUI::Tracked::View::ChartLineBoundaries> LineBoundaries;
typedef TrackedViewImpl<SerialUI::Tracked::View::ChartPie> PieChart;
typedef TrackedViewImpl<SerialUI::Tracked::View::CurrentValue> CurrentValue;
typedef TrackedViewImpl<SerialUI::Tracked::View::HistoryLog> HistoryLog;

}


} /* namespace Item */
} /* namespace Menu */
} /* namespace SerialUI */




#endif /* SERIALUI_SRC_INCLUDES_MENUITEM_TRACKEDVIEW_TRACKEDVIEWS_H_ */

//
//  Test.DebugReport.cpp
//  This file is part of the "Vizor" project and released under the MIT License.
//
//  Created by Samuel Williams on 14/10/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include <UnitTest/UnitTest.hpp>

#include <Vizor/DebugReport.hpp>

namespace Vizor
{
	UnitTest::Suite DebugReportTestSuite {
		"Vizor::DebugReport",
		
		{"it should have some real tests",
			[](UnitTest::Examiner & examiner) {
				examiner.expect(false) == true;
			}
		},
	};
}

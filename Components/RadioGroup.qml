/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7

import "RadioGroup.js" as Private

QtObject {
    id: root
    property variant selectedValue

    function add(item) { Private.add(item); }
    function check(item) { Private.check(item); }
    function select(value) { Private.select(value); }

    onSelectedValueChanged: { select(selectedValue); }
}

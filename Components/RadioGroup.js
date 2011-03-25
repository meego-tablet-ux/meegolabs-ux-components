/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

var items = [];
var selectedItem = null;

function add(item) {
    items.push(item);
    if (selectedValue && item.value === selectedValue) {
        check(item);
    }
}

function check(item) {
    var old = selectedItem;
    selectedItem = item;
    if (old) {
        old.checked = false;
    }
    item.checked = true;
    root.selectedValue = item.value;
}

function select(value) {
    for (var i = 0; i < items.length; i++) {
        if (items[i].value === value) {
            check(items[i]);
            break;
        }
    }
}

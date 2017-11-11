
function TableProcessor() {
    var cellsValues = [];
    var cellsExpressions = [];
    var table = document.getElementById("main_table");
    var aCharCode = 'a'.charCodeAt(0);
    var zCharCode = 'z'.charCodeAt(0);
    var columnCount = zCharCode - aCharCode + 1;
    var rowCount = 20;

    var curInputCell = [];
    var curClickedCell = [];
    var calculateProcessor = new CalculateProcessor(cellsValues);
    var connections=[];
    
    function _initDefault(){
        connections = new Array(rowCount + 1);
        for (var i = 0; i < rowCount + 1; ++i) {
            connections[i] = new Array(columnCount+1);
            for (var j = 0; j < columnCount + 1; ++j) {
                connections[i][j] = new Array();
            }  
        }

        cellsValues = new Array(rowCount + 1);
        for (var i = 0; i < rowCount + 1; ++i) {
            cellsValues[i] = new Array(columnCount+1);
            for (var j = 0; j < columnCount + 1; ++j) {
                cellsValues[i][j] = 0;
            }  
        }
        cellsExpressions = new Array(rowCount + 1);
        for (var i = 0; i < rowCount + 1; ++i) {
            cellsExpressions[i] = new Array(columnCount+1);
            for (var j = 0; j < columnCount + 1; ++j) {
                cellsExpressions[i][j] = "";
            }  
        }

    }

    function _addCell(tr, text) {
        var td = document.createElement('td');
        tr.appendChild(td);
        td.appendChild(document.createTextNode(text)); 
        return td;
    } 

    function GenetateTableHead() {
        var tr = document.createElement('Tr');
        tr.style.backgroundColor = "#efefef";
        _addCell(tr, '');
        j = 'z'.charCodeAt(0);
        for (i = 'a'.charCodeAt(0); i <= j; ++i) {
            _addCell(tr, '' + String.fromCharCode(i));         
        } 
        return tr;
    }

    this.GenerateTable = function() {
        //устанавливаем стили
        table.style.borderCollapse = "collapse";
        table.style.textAlign = "center";

        table.setAttribute("width", "100%");

        var tbody = document.createElement('tbody');
        tbody.appendChild(GenetateTableHead());

        for (var i = 0; i < 20; i++) {
            var tr = document.createElement('tr');
            var td = _addCell(tr, '' + (i+1).toString());
            td.style.backgroundColor = "#efefef";
            
            for (var j = 0; j < columnCount; j++) {
                var td = _addCell(tr, '');
                
                td.setAttribute("contenteditable", "true");
                td.addEventListener('click', (function(i, j) {
                    return function() {OnCellClick(i, j); };
                    })(i+1, j+1), false);
                td.addEventListener('input', (function(i, j) {
                    return function() {OnInput(i, j); };
                    })(i+1, j+1), false);
            }
            tbody.appendChild(tr);
            
        }

        table.appendChild(tbody);
    }

    function OnInput(i, j) {
        curInputCell = [i, j]


        if (table.rows[i].cells[j].innerText.slice(-1) == "\n") {
            table.rows[i].cells[j].innerText = table.rows[i].cells[j].innerText.replace(/(\r\n|\n|\r)/gm,"");
            _setCursorPosition([i+1, j]);
            //curClickedCell = [i + 1, j];
            cellsExpressions[i][j] = table.rows[i].cells[j].innerText;
            OnCellClick(i+1, j);
        } else {
            cellsExpressions[i][j] = table.rows[i].cells[j].innerText;
        }
        if (cellsExpressions[i][j].length > 0 && cellsExpressions[i][j][0] != "=") {
            var value = parseFloat(cellsExpressions[i][j]);
            if (!isNaN(value) && (value.toString().length == cellsExpressions[i][j].length)) {
                cellsValues[i][j] = value;
            } else {
                cellsValues[i][j] = cellsExpressions[i][j];
            }
        }
        saveState();
    }

    function _setCursorPosition(cell) {
        var selectedCell = table.rows[cell[0]].cells[cell[1]];
        var caret = selectedCell.innerText.length;
        if (caret > 0) {
            var textNode = selectedCell.firstChild;
            var range = document.createRange();
            range.setStart(textNode, caret);
            range.setEnd(textNode, caret);
            var sel = window.getSelection();
            sel.removeAllRanges();
            sel.addRange(range);
        } else {
            selectedCell.focus();
        }
    }

    function OnCellClick(i, j){
        var prevClickedCell = curClickedCell;
        curClickedCell = [i, j];  
        if (prevClickedCell.length != curClickedCell.length || prevClickedCell[0] != curClickedCell[0] 
                ||prevClickedCell[1] != curClickedCell[1]) {
            //раскрываем выражение выбранной ячейки
            
                
        

        //кликнули на ячейку, отличную от выбранной
        if ((curInputCell.length != 0) && (curInputCell[0] != i || curInputCell[1] != j)) {
            var curEquation = cellsExpressions[curInputCell[0]][curInputCell[1]];
            if (curEquation.length != 0 && curEquation[0] == '=') {
                lastSymbol = curEquation.slice(-1);
                //перешли на следующую ячейку, надо вычислить выражение той, в которую писали
                if (!isNaN(parseInt(lastSymbol, 10)) || lastSymbol == ")") { 

                    var result = calculateProcessor.calculate(curEquation.slice(1));               
                    table.rows[curInputCell[0]].cells[curInputCell[1]].innerText = result[1];
                    if (result[0] == 1) {
                        cellsValues[curInputCell[0]][curInputCell[1]] = result[1];
                    }
                    var k;
                    var connectedCells = connections[curInputCell[0]][curInputCell[1]];
                    for(k in connectedCells) {
                        var connectedCell = connectedCells[k];
                        
                        var result = calculateProcessor.calculate(cellsExpressions[connectedCell[0]][connectedCell[1]].slice(1));               
                        table.rows[connectedCell[0]].cells[connectedCell[1]].innerText = result[1];
                        if (result[0] == 1) {
                            cellsValues[connectedCell[0]][connectedCell[1]] = result[1];
                        }

                    }
                    
                    //раскрываем выражение выбранной ячейки
                    table.rows[i].cells[j].innerText = cellsExpressions[i][j];
                    _setCursorPosition(curClickedCell);
                    
                } else {
                    //добавляем в строку ссылку на выбранную ячейку
                    newCellRef = String.fromCharCode(aCharCode+ j - 1) + i;
                    connections[i][j].push(curInputCell);     
                    table.rows[curInputCell[0]].cells[curInputCell[1]].innerText += newCellRef;
                    cellsExpressions[curInputCell[0]][curInputCell[1]] += newCellRef;
                    _setCursorPosition(curInputCell);
                }
                saveState();
            } else {
                table.rows[i].cells[j].innerText = cellsExpressions[i][j];
                    _setCursorPosition(curClickedCell);
            }
        } else {
                table.rows[i].cells[j].innerText = cellsExpressions[i][j];
                    _setCursorPosition(curClickedCell);
            }
        }
        curInputCell = [i,j];
    }

    function saveState() {
        localStorage["simple.doc.table.expressions.isSaved"] = true;
        localStorage["simple.doc.table.expressions"] = JSON.stringify(cellsExpressions);
        localStorage["simple.doc.table.values"] = JSON.stringify(cellsValues);
        localStorage["simple.doc.table.connections"] = JSON.stringify(connections);
        return true;
    }

     this.LoadState = function() {
        var isSaved = (localStorage["simple.doc.table.expressions.isSaved"] == "true");
        if (isSaved) {
            cellsExpressions = JSON.parse(localStorage["simple.doc.table.expressions"]);
            cellsValues = JSON.parse(localStorage["simple.doc.table.values"]);
            connections = JSON.parse(localStorage["simple.doc.table.connections"]);
            for(var i in cellsExpressions) {
                for(var j in cellsExpressions[i]) {
                    if (cellsExpressions[i][j].length > 0) {
                        table.rows[i].cells[j].innerText = cellsValues[i][j];
                    }
                }
            }
        } else {
            _initDefault();
        }
        calculateProcessor = new CalculateProcessor(cellsValues);
        return true;
    }

}; 

function CalculateProcessor(cellsValues) {
    var cellsValues = cellsValues;

    const functions = {
        "+": (x, y) => x + y,
        "-": (x, y) => x - y, 
        "*": (x, y) => x * y,
        "/": (x, y) => x / y,
        "SIN": (x) => Math.sin(x),
        "ABS": (x) => Math.abs(x),
        "LEN": function(x) {
            if (typeof x == "string") {
                return x.length - 2;
            } else {
                x.toString().length;
            }
        }
    };

    const argumentCheckers = {
        "+": (x, y) => typeof x == "number" && typeof y == "number",
        "-": (x, y) => typeof x == "number" && typeof y == "number", 
        "*": (x, y) => typeof x == "number" && typeof y == "number",
        "/": (x, y) => typeof x == "number" && typeof y == "number",
        "SIN": (x) => typeof x == "number",
        "ABS": (x) => typeof x == "number",
        "LEN": (x) => typeof x == "string" || typeof x == "number"
    }

    function tokenize(expression) {
        var tokens = expression.split(/([()*+-/])/);
        var token;
        var lexemes = [];
        var i;
        for (i in tokens){
            if (typeof tokens[i] != "undefined" && tokens[i].length != 0) {
                lexemes.push(tokens[i]);
            }
        }
        return lexemes;
    }

    function Parser() {
       this.curPos = 0;       
       this.curNode;  
       this.curParent;                                 
    };

    TreeNode = function() {
        this.left;
        this.right; 
        this.data;
        this.exec;
    }

    Parser.parse = function(expression) {
        try {
           this.curPos = 0;                                       
           return this.parseExp(expression);   
        } catch(e) {
            throw e;
        }                      
    }

    Parser.parseExp = function(expression) {
        try {
            var left = this.parsePrior1(expression);

            var curToken = expression[this.curPos];
            switch (curToken) {
                case "+":
                case "-":
                    var node = new TreeNode();
                    node.left = left;
                    node.data = curToken;
                    ++this.curPos;
                    node.right = this.parseExp(expression);
                    node.exec = function() {
                        var l = this.left.exec();
                        var r = this.right.exec();
                        if (argumentCheckers[this.data](l, r)) {
                            return functions[this.data](l, r);
                        } else {
                            throw new SyntaxError("Invalid argument of " + this.data + " operator");
                        }
                        };
                    return node;
                default:
                    return left;
            }
        } catch (e) {
            throw e;
        }
    }

    Parser.parsePrior1 = function(expression){
        try {
            var left = this.parsePrior2(expression);

            var curToken = expression[this.curPos];
            switch (curToken) {
                case "*":
                case "/":
                    var node = new TreeNode();
                    node.left = left;
                    ++this.curPos;
                    node.data = curToken;
                    node.right = this.parsePrior1(expression);
                    node.exec = function() {
                        var l = this.left.exec();
                        var r = this.right.exec();
                        if (argumentCheckers[this.data](l, r)) {
                            return functions[this.data](l, r);
                        } else {
                            throw new SyntaxError("Invalid argument of" + this.data + "operator");
                        }
                    };
                    return node;
                default:
                    return left;
            }
        } catch (e) {
            throw e;
        }
    }

    Parser.parsePrior2 = function(expression){
        try {
            var curToken = expression[this.curPos];
            if (curToken == "-") {
                ++this.curPos;
                var left = this.parsePrior3(expression);
                var node = new TreeNode();
                node.left = left;
                node.data = curToken;

                node.exec = function() {
                    var l = this.left.exec();
                    if (typeof l == "number") {
                        return -l;
                    } else {
                        throw new SyntaxError("Invalid argument of unary minus " + l);
                    }
                };
                return node; 
            }
            return this.parsePrior3(expression);
        } catch(e) {
            throw e;
        }       
    }

    Parser.parsePrior3 = function(expression){
        try {
            var curToken = expression[this.curPos];
            //имя функции
            if (curToken == "SIN" || curToken == "ABS" || curToken == "LEN") {
                var funcName = curToken;
                ++this.curPos;
                curToken = expression[this.curPos];
                if (curToken == "(") {
                    ++this.curPos;
                    var funcArg = this.parseExp(expression);
                    curToken = expression[this.curPos];
                    if (curToken != ")") {
                        throw new SyntaxError(" ) was missed");
                    } else {
                        ++this.curPos;
                        var node = new TreeNode();
                        node.left = funcArg;
                        node.data = funcName;
                        node.exec = function() {
                            var l = this.left.exec();
                            if (argumentCheckers[this.data](l)) {
                                return functions[this.data](l);
                            } else {
                                throw new SyntaxError("Invalid argument of " + this.data + " function");
                            }
                        };
                        
                        return node;
                    }   
                } else {
                    throw new SyntaxError("The ( after function name was missed");
                }     
            }
            return this.parsePrior4(expression);
        } catch(e) {
            throw e;
        }
    }

    Parser.parsePrior4 = function(expression){
        try {
            //имя ячейки
            var curToken = expression[this.curPos];
            var match = curToken.match(/([a-z])(\d+)/);
            if (match != null && match[0].length == curToken.length) {
                ++this.curPos;
                var node = new TreeNode();
                node.data = curToken;
                node.exec = function() {
                 var cellValue = cellsValues[parseInt(this.data[1], 10)][this.data[0].charCodeAt(0)-'a'.charCodeAt(0)+1];
                 if (typeof cellValue == "string") {
                    return "\"" + cellValue + "\"";
                 } else {
                    return cellValue;
                 }
                  
                };
                return node; 
            } else {
                //число
                var value = parseFloat(curToken);
                if (!isNaN(value) && (value.toString().length == curToken.length)) {
                    ++this.curPos;
                    var node = new TreeNode();
                    node.data = value;
                    node.exec = function() {return this.data;}
                    return node;
                } else {
                    //скобочная конструкция
                    if (curToken == "(") {
                        ++this.curPos;
                        var exp = this.parseExp(expression);
                        curToken = expression[this.curPos];
                        if (curToken != ")") {
                            throw new SyntaxError(" ) was missed");
                        } else {
                            ++this.curPos;
                            return exp;
                        }   
                    }else {        
                        //строка
                        if (curToken.length > 1 && curToken[0]=="\"" && curToken[curToken.length-1] == "\""){
                            ++this.curPos;
                            var node = new TreeNode();
                            node.data = curToken;
                            node.exec = function() {return this.data;}
                            return node;
                        } else {
                            throw new SyntaxError("string should be in brackets");
                        }              
                        
                    }
                }  
            }
        } catch(e) {
            throw e;
        }
          
    }

    this.calculate = function (expression) {
        if (expression.length == 0) {
            return "";
        }
        var result;
        try {
            rootToken = Parser.parse(tokenize(expression));
            result = [1, rootToken.exec()];
        } catch (e) {
            result = [0, "Error:" + e.message + "!!!"];
        }
        return result;
    }
};

function startFunction() {
    var tableProcessor = new TableProcessor();
    tableProcessor.GenerateTable();
    tableProcessor.LoadState();
}

startFunction();

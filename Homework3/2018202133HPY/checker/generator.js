'use strict';

function genInt(a, b)
{
    var x = Math.floor(Math.random() * (b - a) + a);
    return x;
}

function genIMMEStr()
{
    switch (genInt(0, 3)) {
    case 0:
        return genInt(-1000, 1000).toString() + '.' +
            'e' + genInt(-20, 20).toString();

    case 1:
        return (genInt(0, 2) == 0 ? '-' : '+') + '.' + genInt(1, 1000).toString() + 'E' + genInt(-20, 20).toString();

    default:
        return (Math.random() - 0.5) * (Math.random() * Math.random() * 100000).toExponential(5);
    }
}

function replExpr(expr)
{
    var ret = '';
    switch (genInt(0, 3)) {
    case 0:
        ret = expr.replace('EXPR', 'EXPR + TERM')
        break;

    case 1:
        ret = expr.replace('EXPR', 'EXPR - TERM')
        break;

    case 2:
        ret = expr.replace('EXPR', 'TERM')
        break;

    default:
        break;
    }
    return ret;
}

function replTerm(expr)
{
    var ret = '';
    switch (genInt(0, 3)) {
    case 0:
        ret = expr.replace('TERM', 'TERM*FACTOR')
        break;

    case 1:
        ret = expr.replace('TERM', 'TERM/FACTOR')
        break;

    case 2:
        ret = expr.replace('TERM', 'FACTOR')
        break;

    default:
        break;
    }
    return ret;
}

function replFactor(expr)
{
    var ret = '';
    switch (genInt(0, 2)) {
    case 0:
        ret = expr.replace('FACTOR', (x) => genIMMEStr());
        break;

    case 1:
        ret = expr.replace('FACTOR', '(EXPR)');
        break;

    default:
        break;
    }
    return ret;
}

var expr = 'EXPR + TERM / FACTOR';

for (var i = 0; i < 50; i++) {
    expr = replExpr(expr);
    expr = replTerm(expr);
    expr = replFactor(expr);
}

expr = expr.replace(/EXPR/g, 'TERM');
expr = expr.replace(/TERM/g, 'FACTOR');
expr = expr.replace(/FACTOR/g, (x) => genIMMEStr());
expr = expr.replace(/\+ \+/g, '+#+');
expr = expr.replace(/- -/g, '-#-');
expr = expr.replace(/\s/g, '');
expr = expr.replace(/-#-/g, '- -');
expr = expr.replace(/\+#\+/g, '+ +');

//2.3.4
console.log(expr);
console.error(eval(expr));

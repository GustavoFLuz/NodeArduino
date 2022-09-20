const dimensions = {
    cols: 4,
    rows: 4
}
const tabelaDiv = $('#matrizDiv')
const tabela = $('#matriz')
const tbody = tabela.find('tbody')
var code = '';
var cellArray = [];
var cellSelecionada;
resetTable()
btnEvent();
cellControl()

function resetTable() {
    code = ''
    tbody.html('')
    for (let i = 0; i < dimensions.rows; i++) {
        let tr = $('<tr></tr>');
        for (let j = 0; j < dimensions.cols; j++) {
            let td = $('<td></td>')
            let div = $('<div class="cell"></div>')
            td.append(div)
            tr.append(td);
            cellArray[i + ':' + j] = {}
        }
        tbody.append(tr);
    }

    function calcRadius() {
        const tabelaHeight = tabela.height() - 30
        const radius = Math.floor(tabelaHeight / Math.max(dimensions.cols, dimensions.rows))

        $('.cell').each(function () {
            $(this).width(radius);
            $(this).height(radius);
        })
    }
    window.addEventListener('resize', calcRadius)
    calcRadius();
}
function cellControl() {
    var cell;
    $('.cell').click(function (e) {
        e.preventDefault();
        cell = $(e.target);
        if(!cell.hasClass("cell"))
            cell = cell.parents('.cell');
        const linha = cell.parents('tr').index();
        const coluna = cell.parents('td').index();
        cellSelecionada = cellArray[`${linha}:${coluna}`];
        $('#colunaCel').text(coluna + 1);
        $('#linhaCel').text(linha + 1);
    });

    $('#submit').click(function (e) {
        if (cellSelecionada == null) return
        e.preventDefault();
        cell.html('')
        if ($('#group').val() && $('#group').val() != 0) {
            if ($('#inOut').val() == 'in') {
                if ($('#qty').val() && $('#qty').val() != 0) {
                    cellSelecionada.via = $('#inOut').val();
                    cellSelecionada.gp = [{
                        grupo: $('#group').val(),
                        quantidade: $('#qty').val()
                    }]
                    var item = cellSelecionada.gp.length
                    var div = $(
                        `<div class="subitem item${item}">
                            <span class="gp">${$('#group').val()}</span>
                            <span class="qty">${$('#qty').val()} ml</span>
                        </div>`);
                    cell.append(div)
                    cell.removeClass('output');
                    cell.addClass('input')
                } else { alert('Coloque todos os dados necessários') }
            } else {
                cellSelecionada.via = $('#inOut').val();
                cellSelecionada.gp = $('#group').val();
                var div = $(`<span class="gp">${$('#group').val()}</span>`);
                cell.append(div);
                cell.removeClass('input');
                cell.addClass('output');
            }
        } else {
            alert('Coloque todos os dados necessários')
        }
    })
    $('#add').click(function (e) {
        if (cellSelecionada == null) return
        e.preventDefault();
        if ($('#group').val() && $('#group').val() != 0) {
            if ($('#inOut').val() == 'in') {
                var grupoIncluso = cellSelecionada.gp!=undefined?cellSelecionada.gp.filter(el=>el.grupo == $('#group').val()):false;
                if ($('#qty').val() && $('#qty').val() != 0) {
                    if(grupoIncluso.length){
                        grupoIncluso[0].quantidade = $('#qty').val();
                        cell.find('.gp').each((index, el)=>{
                            if($(el).text() == $('#group').val())
                                $(el).parent().find('.qty').text($('#qty').val()+ ' ml')
                        })
                        return;
                    }
                    if(cellSelecionada.via ==undefined) cellSelecionada.via = 'in';
                    if(cellSelecionada.gp  ==undefined) cellSelecionada.gp = [];
                    cellSelecionada.gp.push({
                        grupo: $('#group').val(),
                        quantidade: $('#qty').val()
                    })
                    var item = cellSelecionada.gp.length
                    var div = $(
                        `<div class="subitem item${item}">
                            <span class="gp">${$('#group').val()}</span>
                            <span class="qty">${$('#qty').val()} ml</span>
                        </div>`);
                    cell.append(div);
                    cell.addClass('input')
                } else { alert('Coloque todos os dados necessários') }
            } else {
                alert('Não pode adicionar itens a saídas')
            }
        } else {
            alert('Coloque todos os dados necessários')
        }
    })
    $('#resetCell').click(function (e) {
        cellSelecionada = {};
        cell.html('');
        cell.removeClass('input');
        cell.removeClass('output');

    })
}
function btnEvent() {
    $('#reset').click(resetTable)
    $('#run').click(() => {
        code = ''
        const grupos = []
        code += dimensions.rows + 'x' + dimensions.cols + '-.';
        Object.keys(cellArray).forEach((key) => {
            var cell = cellArray[key]
            if (cell != {})
                if(cell.via == 'out') grupos[cell.gp] = {out:key, in:[]}
        })
        Object.keys(cellArray).forEach((key) => {
            var cell = cellArray[key]
            if (cell != {})
                if(cell.via == 'in'){
                    var inputs = cell.gp;
                    inputs.forEach(input=>{
                        if(grupos[input.grupo] == undefined) return;
                        grupos[input.grupo].in.push(key+'='+input.quantidade)
                    })
                }
        })
        grupos.forEach((grupo, index) => {
            var str = '';
            str += grupo.out + '-';
            grupo.in.forEach(input => {
                str += input + '-'
            })
            str += '.'
            code += str;
        })
        postString(code)
    })
}
var intervalCode;
function postString(code) {
    let xhr = new XMLHttpRequest();
    xhr.open('POST', '/' + code);
    xhr.setRequestHeader('Content-Type', 'application/json');
    xhr.send();
    intervalCode = setInterval(getString, 50);
}
function getString(){
    let xhr = new XMLHttpRequest();
    xhr.open('GET', '/str');
    xhr.onload = ()=> printString(xhr.response);
    xhr.send();
}
var currPos = '';
var oldRead = "";
function printString(pos){
    if(oldRead == pos) return;
    oldRead = pos;
    if(pos[0]== 'C'){
        if(currPos == pos) return;
        currPos = pos;
        currRow = parseInt(currPos.substring(1).split('-')[0])
        currCol = parseInt(currPos.split('-')[1]);
        $('.cell').removeClass('active')
        $('#matriz').find(`tr:nth-child(${currRow+1})>td:nth-child(${currCol+1})>.cell`).addClass('active')
    } else if(pos[0] == "P"){
        $('.active').addClass('pick');
        setTimeout(()=>{$('.pick').removeClass('pick')}, 200)
    } 
    else if(pos == "Over"){
        clearInterval(intervalCode)
    }
}
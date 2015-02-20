#include "IFXFancyTable.h"
#include <FXTable.h>

// Message Map
FXDEFMAP(IFXFancyTable) IFXFancyTableMap[]= {
	FXMAPFUNC(SEL_PAINT, 0, IFXFancyTable::onPaint)
};

FXIMPLEMENT(IFXFancyTable, FXTable, IFXFancyTableMap, ARRAYNUMBER(IFXFancyTableMap))
//FXIMPLEMENT(IFXFancyTable, FXTable, NULL, 0)

IFXFancyTable::IFXFancyTable(FXComposite* parent, FXObject* tgt, FXSelector sel, FXuint opts, FXint x, FXint y, FXint w, FXint h)
: FXTable(parent, tgt, sel, opts, x,y,w,h) // Parent-Konstruktor aufrufen
{
	/* Table initialisierung */
	setEditable(FALSE);
	showVertGrid(TRUE);

	setRowHeaderWidth(0); // graue Spalte Links verstecken

	// sichtbare Zeilen setzen
	setVisibleRows(0);

	FXint light = FXRGB(0xDB, 0xDB, 0xDB);
	FXint dark = FXRGB(0xF0, 0xF0, 0xF0);

	// gerade Zeilen hell
	setCellColor(0, 0, light); 
	setCellColor(0, 1, light);

	// ungerade zeilen dunkel
	setCellColor(1, 0, dark);
	setCellColor(1, 1, dark);
}

IFXFancyTable::~IFXFancyTable() {}

void IFXFancyTable::setTableSize(FXint nr, FXint nc, FXbool notify)
{
	cweight.resize(nc);
	for(int c=0; c<nc; c++) cweight[c] = 1;

	FXTable::setTableSize(nr, nc, notify);
	setVisibleColumns(nc); 

	//for(int c=0; c<nr; c++) getRowHeader()->setItemJustify(c, FXHeaderItem::LEFT);/*getColumnHeader()->setItemJustify(c, FXHeaderItem::LEFT);*/

}

double IFXFancyTable::getColumnWeight(FXint column) const {
	return ((column > 0) && (column < getNumColumns()) ? cweight[column] : 0);
}

void IFXFancyTable::setColumnWeight(FXint column, double dWeight) {

	if ((column >= 0) && (column < getNumColumns()) && (dWeight > 0)) {
		cweight[column] = dWeight;
		layout();
	}
}

void IFXFancyTable::layout()
{
	FXMutexLock oLock(mxTable);

	int nc = getNumColumns();
	int width = getWidth();
	int sum = 0;
	double csum = 0;
	
	// Gewichtungsfaktoren:
	for (int i=0; i<nc ; i++) csum += cweight[i];

	for (int i=0; i<(nc-1); i++)
	{
		int cwidth = (int) ((double) width * cweight[i] / csum);
		setColumnWidth(i, cwidth);			
		sum += cwidth;
	}

	// Headeritem Justify
	for (FXint r=0; r < getNumRows() ;r++) {
		for (FXint c=0; c<getNumColumns() ;c++) {
			setItemJustify(r, c, FXTableItem::LEFT);
		}
	}

	// Die letzte Spalte bekommt den Rest (-1 braucht man, sonst Scrollbar)
	setColumnWidth(nc-1, width-sum-1);

	// all headerItems to Left-Justifed
	for (FXint r=0;r<getNumRows();r++) 
	{
		for (FXint c=0; c<getNumColumns(); c++) 
		{
			setItemJustify(r, c, FXTableItem::LEFT);
		}
	}
	
	// Methode der Oberklasse aufrufen
	FXTable::layout();
}

FXbool IFXFancyTable::selectRow(FXint row, FXbool notify)
{
	return true;
}

FXbool IFXFancyTable::selectColumn(FXint col, FXbool notify)
{
	return false;
}

FXbool IFXFancyTable::selectRange(FXint startrow, FXint endrow, FXint startcol, FXint endcol, FXbool notify)
{
	FXTable::selectRange(startrow, startrow, 0, getNumColumns()-1, FALSE);

	return 1;
}

long IFXFancyTable::onCmd(FX::FXObject *, FX::FXSelector, void *)
{
	return 1;
}

long IFXFancyTable::onPaint(FXObject* sender, FXSelector sel, void* param)
{
	// Call superclass paint method (thread-safe)
	FXMutexLock oLock(mxTable);
	return FXTable::onPaint(sender, sel, param);
}

void IFXFancyTable::insertRows(FXint row,FXint nr,FXbool notify) {
	FXMutexLock oLock(mxTable);
	FXTable::insertRows(row, nr, notify);
}

void IFXFancyTable::insertColumns(FXint col,FXint nc,FXbool notify) {
	FXMutexLock oLock(mxTable);
	FXTable::insertColumns(col, nc, notify);
}

void IFXFancyTable::removeRows(FXint row,FXint nr,FXbool notify) {
	FXMutexLock oLock(mxTable);
	FXTable::removeRows(row, nr, notify);
}

void IFXFancyTable::removeColumns(FXint col,FXint nc,FXbool notify) {
	FXMutexLock oLock(mxTable);
	FXTable::removeColumns(col, nc, notify);
}

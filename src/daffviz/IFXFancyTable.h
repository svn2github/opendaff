#ifndef __IFX_FANCYTABLE_H__
#define __IFX_FANCYTABLE_H__

// Fox-Includes
#include <fx.h>
#include <vector>

class IFXFancyTable : public FXTable {
	FXDECLARE(IFXFancyTable)


public:
	
	enum {
		ID_LAST = FXTable::ID_LAST
    };

	// Konstraktor und Dekonstraktor
	IFXFancyTable(FXComposite* parent, FXObject* tgt=NULL, FXSelector sel=0,FXuint opts=0, FXint x=0, FXint y=0, FXint w=0, FXint h=0);
	virtual ~IFXFancyTable();

	// Anzahl der Zeilen und Spalten setzen
	virtual void setTableSize(FXint nr, FXint nc, FXbool notify=FALSE);

	// Gewicht (Breite) einer Spalte zurückgeben
	virtual double getColumnWeight(FXint column) const;

	// Gewicht (Breite) einer Spalte setzen (Wert > 0)
	virtual void setColumnWeight(FXint column, double dWeight);

	/* Von FXTable überladene Layout-Methode. Paßt Spaltenbreiten dynamisch an. */
	void layout();

	virtual long onCmd(FXObject*, FXSelector, void*);
	virtual long onPaint(FXObject*, FXSelector, void*);

	/* +------------------------------------+
	 * |                                    |
	 * |  Range in der Tabelle selektieren  |
	 * |                                    |
	 * +------------------------------------+ */

	virtual FXbool 	selectRow  (FXint row, FXbool  notify=FALSE);
	virtual FXbool 	selectColumn (FXint col, FXbool notify=FALSE);
	virtual FXbool 	selectRange (FXint startrow, FXint endrow, FXint startcol, FXint endcol, FXbool notify=FALSE);

	// Nachträgliche Synchronisierung einer Thread-unsicherer Methoden
	// TODO: Das sind nicht alle. Bei Bedarf erweitern...

	virtual void insertRows(FXint row,FXint nr=1,FXbool notify=FALSE);
	virtual void insertColumns(FXint col,FXint nc=1,FXbool notify=FALSE);
	virtual void removeRows(FXint row,FXint nr=1,FXbool notify=FALSE);
	virtual void removeColumns(FXint col,FXint nc=1,FXbool notify=FALSE);

protected:
	IFXFancyTable() {};

	std::vector<double> cweight;
	FXMutex mxTable;				// Lock für exklusive Änderung am Tabellen-Layout (z.B. Zeilen löschen)
};

#endif // __IFX_FANCYTABLE_H__

/* poppler-annotation.h: qt interface to poppler
 * Copyright (C) 2006-2008, 2012, 2013, 2018-2022 Albert Astals Cid <aacid@kde.org>
 * Copyright (C) 2006, 2008 Pino Toscano <pino@kde.org>
 * Copyright (C) 2007, Brad Hards <bradh@frogmouth.net>
 * Copyright (C) 2010, Philip Lorenz <lorenzph+freedesktop@gmail.com>
 * Copyright (C) 2012, 2015, Tobias Koenig <tobias.koenig@kdab.com>
 * Copyright (C) 2012, Guillermo A. Amaral B. <gamaral@kde.org>
 * Copyright (C) 2012, 2013 Fabio D'Urso <fabiodurso@hotmail.it>
 * Copyright (C) 2013, Anthony Granger <grangeranthony@gmail.com>
 * Copyright (C) 2018, Dileep Sankhla <sankhla.dileep96@gmail.com>
 * Copyright (C) 2020, Katarina Behrens <Katarina.Behrens@cib.de>
 * Copyright (C) 2020, 2024, Klarälvdalens Datakonsult AB, a KDAB Group company, <info@kdab.com>. Work sponsored by Technische Universität Dresden
 * Copyright (C) 2021, Oliver Sander <oliver.sander@tu-dresden.de>
 * Copyright (C) 2021, Mahmoud Ahmed Khalil <mahmoudkhalil11@gmail.com>
 * Copyright (C) 2024, 2025, g10 Code GmbH, Author: Sune Stolborg Vuorela <sune@vuorela.dk>
 * Adapting code from
 *   Copyright (C) 2004 by Enrico Ros <eros.kde@email.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef _POPPLER_ANNOTATION_H_
#define _POPPLER_ANNOTATION_H_

#include <QtCore/QDateTime>
#include <QtCore/QSharedDataPointer>
#include <QtCore/QList>
#include <QtCore/QPointF>
#include <QtCore/QRectF>
#include <QtCore/QScopedPointer>
#include <QtCore/QVector>
#include <QtGui/QColor>
#include <QtGui/QFont>
#include "poppler-export.h"
#include "poppler-converter.h"

#include <memory>

namespace Poppler {

class Annotation;
class AnnotationPrivate;
class AnnotationAppearancePrivate;
class TextAnnotationPrivate;
class LineAnnotationPrivate;
class GeomAnnotationPrivate;
class HighlightAnnotationPrivate;
class StampAnnotationPrivate;
class SignatureAnnotationPrivate;
class InkAnnotationPrivate;
class LinkAnnotationPrivate;
class CaretAnnotationPrivate;
class FileAttachmentAnnotationPrivate;
class SoundAnnotationPrivate;
class MovieAnnotationPrivate;
class ScreenAnnotationPrivate;
class WidgetAnnotationPrivate;
class RichMediaAnnotationPrivate;
class EmbeddedFile;
class Link;
class SoundObject;
class MovieObject;
class LinkRendition;
class Page;

/**
 * \short AnnotationAppearance class wrapping Poppler's AP stream object
 *
 * The Annotation's Appearance Stream is a Form XObject containing
 * information required to properly render the Annotation on the document.
 *
 * This class wraps Poppler's Object implementing the appearance stream
 * for the calling annotation. It can be used to preserve the current
 * Appearance Stream for the calling annotation.
 *
 * \since 21.10.0
 */
class POPPLER_QT6_EXPORT AnnotationAppearance
{
    friend class Annotation;

public:
    explicit AnnotationAppearance(AnnotationAppearancePrivate *annotationAppearancePrivate);
    ~AnnotationAppearance();

private:
    AnnotationAppearancePrivate *d;
    Q_DISABLE_COPY(AnnotationAppearance)
};

/**
 * \short Annotation class holding properties shared by all annotations.
 *
 * An Annotation is an object (text note, highlight, sound, popup window, ..)
 * contained by a Page in the document.
 *
 * \warning Different Annotation objects might point to the same annotation.
 *
 * \section annotCreation How to add annotations
 *
 * Create an Annotation object of the desired subclass (for example
 * TextAnnotation) and set its properties:
 * @code
 * Poppler::TextAnnotation* myann = new Poppler::TextAnnotation(Poppler::TextAnnotation::InPlace);
 * myann->setBoundary(QRectF(0.1, 0.1, 0.2, 0.2)); // normalized coordinates: (0,0) is top-left, (1,1) is bottom-right
 * myann->setContents("Hello, world!");
 * @endcode
 * \note Always set a boundary rectangle, or nothing will be shown!
 *
 * Obtain a pointer to the Page where you want to add the annotation (refer to
 * \ref req for instructions) and add the annotation:
 * @code
 * Poppler::Page* mypage = ...;
 * mypage->addAnnotation(myann);
 * @endcode
 *
 * You can keep on editing the annotation after it has been added to the page:
 * @code
 * myann->setContents("World, hello!"); // Let's change text...
 * myann->setAuthor("Your name here");  // ...and set an author too
 * @endcode
 *
 * When you're done with editing the annotation, you must destroy the Annotation
 * object:
 * @code
 * delete myann;
 * @endcode
 *
 * Use the PDFConverter class to save the modified document.
 *
 * \section annotFixedRotation FixedRotation flag specifics
 *
 * According to the PDF specification, annotations whose
 * Annotation::FixedRotation flag is set must always be shown in their original
 * orientation, no matter what the current rendering rotation or the page's
 * Page::orientation() values are. In comparison with regular annotations, such
 * annotations should therefore be transformed by an extra rotation at rendering
 * time to "undo" such context-related rotations, which is equal to
 * <code>-(rendering_rotation + page_orientation)</code>. The rotation pivot
 * is the top-left corner of the boundary rectangle.
 *
 * In practice, %Poppler's \ref Page::renderToImage only "unrotates" the
 * page orientation, and does <b>not</b> unrotate the rendering rotation.
 * This ensures consistent renderings at different Page::Rotation values:
 * annotations are always positioned as if they were being positioned at the
 * default page orientation.
 *
 * Just like regular annotations, %Poppler Qt6 exposes normalized coordinates
 * relative to the page's default orientation. However, behind the scenes, the
 * coordinate system is different and %Poppler transparently transforms each
 * shape. If you never call either Annotation::setFlags or
 * Annotation::setBoundary, you don't need to worry about this; but if you do
 * call them, then you need to adhere to the following rules:
 *  - Whenever you toggle the Annotation::FixedRotation flag, you <b>must</b>
 *    set again the boundary rectangle first, and then you <b>must</b> set
 *    again any other geometry-related property.
 *  - Whenever you modify the boundary rectangle of an annotation whose
 *    Annotation::FixedRotation flag is set, you <b>must</b> set again any other
 *    geometry-related property.
 *
 * These two rules are necessary to make %Poppler's transparent coordinate
 * conversion work properly.
 */
class POPPLER_QT6_EXPORT Annotation
{
    friend class LinkMovie;
    friend class LinkRendition;

public:
    // enum definitions
    /**
     * Annotation subclasses
     *
     * \sa subType()
     */
    // WARNING!!! oKular uses that very same values so if you change them notify the author!
    enum SubType
    {
        AText = 1, ///< TextAnnotation
        ALine = 2, ///< LineAnnotation
        AGeom = 3, ///< GeomAnnotation
        AHighlight = 4, ///< HighlightAnnotation
        AStamp = 5, ///< StampAnnotation
        AInk = 6, ///< InkAnnotation
        ALink = 7, ///< LinkAnnotation
        ACaret = 8, ///< CaretAnnotation
        AFileAttachment = 9, ///< FileAttachmentAnnotation
        ASound = 10, ///< SoundAnnotation
        AMovie = 11, ///< MovieAnnotation
        AScreen = 12, ///< ScreenAnnotation
        AWidget = 13, ///< WidgetAnnotation
        ARichMedia = 14 ///< RichMediaAnnotation
    };

    /**
     * Annotation flags
     *
     * They can be OR'd together (e.g. Annotation::FixedRotation | Annotation::DenyPrint).
     *
     * \sa flags(), setFlags()
     */
    // NOTE: Only flags that are known to work are documented
    enum Flag
    {
        Hidden = 1, ///< Do not display or print the annotation
        FixedSize = 2,
        FixedRotation = 4, ///< Do not rotate the annotation according to page orientation and rendering rotation \warning Extra care is needed with this flag: see \ref annotFixedRotation
        DenyPrint = 8, ///< Do not print the annotation
        DenyWrite = 16,
        DenyDelete = 32,
        ToggleHidingOnMouse = 64,
        External = 128
    };
    Q_DECLARE_FLAGS(Flags, Flag)

    enum LineStyle
    {
        Solid = 1,
        Dashed = 2,
        Beveled = 4,
        Inset = 8,
        Underline = 16
    };
    enum LineEffect
    {
        NoEffect = 1,
        Cloudy = 2
    };
    enum RevScope
    {
        Root = 0,
        Reply = 1,
        Group = 2,
        Delete = 4
    };
    enum RevType
    {
        None = 1,
        Marked = 2,
        Unmarked = 4,
        Accepted = 8,
        Rejected = 16,
        Cancelled = 32,
        Completed = 64
    };

    /**
     * Returns the author of the annotation.
     */
    QString author() const;
    /**
     * Sets a new author for the annotation.
     */
    void setAuthor(const QString &author);

    QString contents() const;
    void setContents(const QString &contents);

    /**
     * Returns the unique name (ID) of the annotation.
     */
    QString uniqueName() const;
    /**
     * Sets a new unique name for the annotation.
     *
     * \note no check of the new uniqueName is done
     */
    void setUniqueName(const QString &uniqueName);

    QDateTime modificationDate() const;
    void setModificationDate(const QDateTime &date);

    QDateTime creationDate() const;
    void setCreationDate(const QDateTime &date);

    /**
     * Returns this annotation's flags
     *
     * \sa Flag, setFlags()
     */
    Flags flags() const;
    /**
     * Sets this annotation's flags
     *
     * \sa Flag, flags(), \ref annotFixedRotation
     */
    void setFlags(Flags flags);

    /**
     * Returns this annotation's boundary rectangle in normalized coordinates
     *
     * \sa setBoundary(const QRectF&)
     */
    QRectF boundary() const;
    /**
     * Sets this annotation's boundary rectangle
     *
     * The boundary rectangle is the smallest rectangle that contains the
     * annotation.
     *
     * \warning This property is mandatory: you must always set this.
     *
     * \sa boundary(), \ref annotFixedRotation
     */
    void setBoundary(const QRectF &boundary);

    /**
     * \short Container class for Annotation style information
     */
    class POPPLER_QT6_EXPORT Style
    {
    public:
        Style();
        Style(const Style &other);
        Style &operator=(const Style &other);
        ~Style();

        // appearance properties
        QColor color() const; // black
        void setColor(const QColor &color);
        double opacity() const; // 1.0
        void setOpacity(double opacity);

        // pen properties
        double width() const; // 1.0
        void setWidth(double width);
        LineStyle lineStyle() const; // LineStyle::Solid
        void setLineStyle(LineStyle style);
        double xCorners() const; // 0.0
        void setXCorners(double radius);
        double yCorners() const; // 0.0
        void setYCorners(double radius);
        const QVector<double> &dashArray() const; // [ 3 ]
        void setDashArray(const QVector<double> &array);

        // pen effects
        LineEffect lineEffect() const; // LineEffect::NoEffect
        void setLineEffect(LineEffect effect);
        double effectIntensity() const; // 1.0
        void setEffectIntensity(double intens);

    private:
        class Private;
        QSharedDataPointer<Private> d;
    };

    Style style() const;
    void setStyle(const Style &style);

    /**
     * \short Container class for Annotation pop-up window information
     */
    class POPPLER_QT6_EXPORT Popup
    {
    public:
        Popup();
        Popup(const Popup &other);
        Popup &operator=(const Popup &other);
        ~Popup();

        // window state (Hidden, FixedRotation, Deny* flags allowed)
        int flags() const; // -1 (never initialized) -> 0 (if inited and shown)
        void setFlags(int flags);

        // geometric properties
        QRectF geometry() const; // no default
        void setGeometry(const QRectF &geom);

        // window contents/override properties
        QString title() const; // '' text in the titlebar (overrides author)
        void setTitle(const QString &title);
        QString summary() const; // '' short description (displayed if not empty)
        void setSummary(const QString &summary);
        QString text() const; // '' text for the window (overrides annot->contents)
        void setText(const QString &text);

    private:
        class Private;
        QSharedDataPointer<Private> d;
    };

    Popup popup() const;
    /// \warning Currently does nothing \since 0.20
    void setPopup(const Popup &popup);

    RevScope revisionScope() const; // Root

    RevType revisionType() const; // None

    /**
     * Returns the revisions of this annotation
     */
    std::vector<std::unique_ptr<Annotation>> revisions() const;

    /**
     * The type of the annotation.
     */
    virtual SubType subType() const = 0;

    /**
     * Returns the current appearance stream of this annotation.
     *
     * \since 21.10.0
     */
    std::unique_ptr<AnnotationAppearance> annotationAppearance() const;

    /**
     * Sets the annotation's appearance stream with the @p annotationAppearance.
     *
     * \since 21.10.0
     */
    void setAnnotationAppearance(const AnnotationAppearance &annotationAppearance);

    /**
     * Destructor.
     */
    virtual ~Annotation();

    /**
     * Describes the flags from an annotations 'AA' dictionary.
     *
     * This flag is used by the additionalAction() method for ScreenAnnotation
     * and WidgetAnnotation.
     */
    enum AdditionalActionType
    {
        CursorEnteringAction, ///< Performed when the cursor enters the annotation's active area
        CursorLeavingAction, ///< Performed when the cursor exists the annotation's active area
        MousePressedAction, ///< Performed when the mouse button is pressed inside the annotation's active area
        MouseReleasedAction, ///< Performed when the mouse button is released inside the annotation's active area
        FocusInAction, ///< Performed when the annotation receives the input focus
        FocusOutAction, ///< Performed when the annotation loses the input focus
        PageOpeningAction, ///< Performed when the page containing the annotation is opened
        PageClosingAction, ///< Performed when the page containing the annotation is closed
        PageVisibleAction, ///< Performed when the page containing the annotation becomes visible
        PageInvisibleAction ///< Performed when the page containing the annotation becomes invisible
    };

protected:
    /// \cond PRIVATE
    explicit Annotation(AnnotationPrivate &dd);
    Q_DECLARE_PRIVATE(Annotation)
    QExplicitlySharedDataPointer<AnnotationPrivate> d_ptr;
    /// \endcond

private:
    Q_DISABLE_COPY(Annotation)
};

/**
 * \short Annotation containing text.
 *
 * A text annotation is an object showing some text directly on the page, or
 * linked to the contents using an icon shown on a page.
 */
class POPPLER_QT6_EXPORT TextAnnotation : public Annotation
{
    friend class AnnotationPrivate;

public:
    // local enums
    enum TextType
    {
        Linked,
        InPlace
    };
    enum InplaceIntent
    {
        Unknown,
        Callout,
        TypeWriter
    };
    enum InplaceAlignPosition
    {
        InplaceAlignLeft,
        InplaceAlignCenter,
        InplaceAlignRight
    };

    explicit TextAnnotation(TextType type);
    ~TextAnnotation() override;
    SubType subType() const override;

    /**
       The type of text annotation represented by this object
    */
    TextType textType() const;

    /**
       The name of the icon for this text annotation.

       Standard names for text annotation icons are:
       - Comment
       - Help
       - Insert
       - Key
       - NewParagraph
       - Note (this is the default icon to use)
       - Paragraph
    */
    QString textIcon() const;

    /**
       Set the name of the icon to use for this text annotation.

       \sa textIcon for the list of standard names
    */
    void setTextIcon(const QString &icon);

    QFont textFont() const;
    void setTextFont(const QFont &font);
    /// Default text color is black
    QColor textColor() const;
    void setTextColor(const QColor &color);

    InplaceAlignPosition inplaceAlign() const;
    void setInplaceAlign(InplaceAlignPosition align);

    QPointF calloutPoint(int id) const;
    QVector<QPointF> calloutPoints() const;
    void setCalloutPoints(const QVector<QPointF> &points);

    InplaceIntent inplaceIntent() const;
    void setInplaceIntent(InplaceIntent intent);

private:
    explicit TextAnnotation(TextAnnotationPrivate &dd);
    void setTextType(TextType type);
    Q_DECLARE_PRIVATE(TextAnnotation)
    Q_DISABLE_COPY(TextAnnotation)
};

/**
 * \short Polygon/polyline annotation.
 *
 * This annotation represents a polygon (or polyline) to be drawn on a page.
 */
class POPPLER_QT6_EXPORT LineAnnotation : public Annotation
{
    friend class AnnotationPrivate;

public:
    // local enums
    enum LineType
    {
        StraightLine,
        Polyline
    };
    enum TermStyle
    {
        Square,
        Circle,
        Diamond,
        OpenArrow,
        ClosedArrow,
        None,
        Butt,
        ROpenArrow,
        RClosedArrow,
        Slash
    };
    enum LineIntent
    {
        Unknown,
        Arrow,
        Dimension,
        PolygonCloud
    };

    explicit LineAnnotation(LineType type);
    ~LineAnnotation() override;
    SubType subType() const override;

    LineType lineType() const;

    QVector<QPointF> linePoints() const;
    void setLinePoints(const QVector<QPointF> &points);

    TermStyle lineStartStyle() const;
    void setLineStartStyle(TermStyle style);

    TermStyle lineEndStyle() const;
    void setLineEndStyle(TermStyle style);

    bool isLineClosed() const;
    void setLineClosed(bool closed);

    QColor lineInnerColor() const;
    void setLineInnerColor(const QColor &color);

    double lineLeadingForwardPoint() const;
    void setLineLeadingForwardPoint(double point);

    double lineLeadingBackPoint() const;
    void setLineLeadingBackPoint(double point);

    bool lineShowCaption() const;
    void setLineShowCaption(bool show);

    LineIntent lineIntent() const;
    void setLineIntent(LineIntent intent);

private:
    explicit LineAnnotation(LineAnnotationPrivate &dd);
    void setLineType(LineType type);
    Q_DECLARE_PRIVATE(LineAnnotation)
    Q_DISABLE_COPY(LineAnnotation)
};

/**
 * \short Geometric annotation.
 *
 * The geometric annotation represents a geometric figure, like a rectangle or
 * an ellipse.
 */
class POPPLER_QT6_EXPORT GeomAnnotation : public Annotation
{
    friend class AnnotationPrivate;

public:
    GeomAnnotation();
    ~GeomAnnotation() override;
    SubType subType() const override;

    // common enums
    enum GeomType
    {
        InscribedSquare,
        InscribedCircle
    };

    GeomType geomType() const;
    void setGeomType(GeomType type);

    QColor geomInnerColor() const;
    void setGeomInnerColor(const QColor &color);

private:
    explicit GeomAnnotation(GeomAnnotationPrivate &dd);
    Q_DECLARE_PRIVATE(GeomAnnotation)
    Q_DISABLE_COPY(GeomAnnotation)
};

/**
 * \short Text highlight annotation.
 *
 * The highlight annotation represents some areas of text being "highlighted".
 */
class POPPLER_QT6_EXPORT HighlightAnnotation : public Annotation
{
    friend class AnnotationPrivate;

public:
    HighlightAnnotation();
    ~HighlightAnnotation() override;
    SubType subType() const override;

    /**
       The type of highlight
    */
    enum HighlightType
    {
        Highlight, ///< highlighter pen style annotation
        Squiggly, ///< jagged or squiggly underline
        Underline, ///< straight line underline
        StrikeOut ///< straight line through-line
    };

    /**
       Structure corresponding to a QuadPoints array. This matches a
       quadrilateral that describes the area around a word (or set of
       words) that are to be highlighted.
    */
    struct Quad
    {
        QPointF points[4]; // 8 valid coords
        bool capStart; // false (vtx 1-4) [K]
        bool capEnd; // false (vtx 2-3) [K]
        double feather; // 0.1 (in range 0..1) [K]
    };

    /**
       The type (style) of highlighting to use for this area
       or these areas.
    */
    HighlightType highlightType() const;

    /**
       Set the type of highlighting to use for the given area
       or areas.
    */
    void setHighlightType(HighlightType type);

    /**
       The list of areas to highlight.
    */
    QList<Quad> highlightQuads() const;

    /**
       Set the areas to highlight.
    */
    void setHighlightQuads(const QList<Quad> &quads);

private:
    explicit HighlightAnnotation(HighlightAnnotationPrivate &dd);
    Q_DECLARE_PRIVATE(HighlightAnnotation)
    Q_DISABLE_COPY(HighlightAnnotation)
};

/**
 * \short Stamp annotation.
 *
 * A simple annotation drawing a stamp on a page.
 */
class POPPLER_QT6_EXPORT StampAnnotation : public Annotation
{
    friend class AnnotationPrivate;

public:
    StampAnnotation();
    ~StampAnnotation() override;
    SubType subType() const override;

    /**
       The name of the icon for this stamp annotation.

       Standard names for stamp annotation icons are:
       - Approved
       - AsIs
       - Confidential
       - Departmental
       - Draft (this is the default icon type)
       - Experimental
       - Expired
       - Final
       - ForComment
       - ForPublicRelease
       - NotApproved
       - NotForPublicRelease
       - Sold
       - TopSecret
    */
    QString stampIconName() const;

    /**
       Set the icon type for this stamp annotation.

       \sa stampIconName for the list of standard icon names
    */
    void setStampIconName(const QString &name);

    /**
       Set a custom icon for this stamp annotation.

       \since 21.10.0
    */
    void setStampCustomImage(const QImage &image);

private:
    explicit StampAnnotation(StampAnnotationPrivate &dd);
    Q_DECLARE_PRIVATE(StampAnnotation)
    Q_DISABLE_COPY(StampAnnotation)
};

/**
 * \short Signature annotation.
 *
 * A signature annotation. By creating one and adding it to the page you can create
 * an unsigned signature field.
 *
 * To read existing signature fields use FormFieldSignature.
 *
 * \since 24.10
 */
class POPPLER_QT6_EXPORT SignatureAnnotation : public Annotation
{
    friend class AnnotationPrivate;

public:
    /**
     * \since 24.10
     */
    enum SigningResult
    {
        SigningSuccess, ///< No error
        FieldAlreadySigned, ///< Trying to sign a field that is already signed
        GenericSigningError, ///< Unclassified error
        InternalError, ///< Unexpected error, likely a bug in poppler \since 24.12
        KeyMissing, ///< Key not found (Either the input key is not from the list or the available keys has changed underneath) \since 24.12
        WriteFailed, ///< Write failed (permissions, faulty disk, ...) \since 24.12
        UserCancelled, ///< User cancelled the process \since 24.12
        BadPassphrase, ///< User entered bad passphrase \since 25.03
    };

    SignatureAnnotation();
    ~SignatureAnnotation() override;
    SubType subType() const override;

    void setText(const QString &text);
    void setLeftText(const QString &text);

    /**
     * Default: 10
     */
    double fontSize() const;
    void setFontSize(double fontSize);

    /**
     * Default: 20
     */
    double leftFontSize() const;
    void setLeftFontSize(double fontSize);

    /**
     * Default: red
     */
    QColor fontColor() const;
    void setFontColor(const QColor &color);

    /**
     * Default: red
     */
    QColor borderColor() const;
    void setBorderColor(const QColor &color);

    /**
     * border width in points
     *
     * Default: 1.5
     */
    double borderWidth() const;
    void setBorderWidth(double width);

    /**
     * Default: QColor(240, 240, 240)
     */
    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &color);

    QString imagePath() const;
    void setImagePath(const QString &imagePath);

    QString fieldPartialName() const;
    void setFieldPartialName(const QString &fieldPartialName);

    [[nodiscard]] SigningResult sign(const QString &outputFileName, const PDFConverter::NewSignatureData &data);

    /**
     * A string with a string that might offer more details of the signing result failure
     * \note the string here is likely not super useful for end users, but might give more details to a trained supporter / bug triager
     * \since 25.07
     */
    ErrorString lastSigningErrorDetails() const;

private:
    explicit SignatureAnnotation(SignatureAnnotationPrivate &dd);
    Q_DECLARE_PRIVATE(SignatureAnnotation)
    Q_DISABLE_COPY(SignatureAnnotation)
};

/**
 * \short Ink Annotation.
 *
 * Annotation representing an ink path on a page.
 */
class POPPLER_QT6_EXPORT InkAnnotation : public Annotation
{
    friend class AnnotationPrivate;

public:
    InkAnnotation();
    ~InkAnnotation() override;
    SubType subType() const override;

    QList<QVector<QPointF>> inkPaths() const;
    void setInkPaths(const QList<QVector<QPointF>> &paths);

private:
    explicit InkAnnotation(InkAnnotationPrivate &dd);
    Q_DECLARE_PRIVATE(InkAnnotation)
    Q_DISABLE_COPY(InkAnnotation)
};

class POPPLER_QT6_EXPORT LinkAnnotation : public Annotation
{
    friend class AnnotationPrivate;

public:
    ~LinkAnnotation() override;
    SubType subType() const override;

    // local enums
    enum HighlightMode
    {
        None,
        Invert,
        Outline,
        Push
    };

    Link *linkDestination() const;
    void setLinkDestination(std::unique_ptr<Link> &&link);

    HighlightMode linkHighlightMode() const;
    void setLinkHighlightMode(HighlightMode mode);

    QPointF linkRegionPoint(int id) const;
    void setLinkRegionPoint(int id, const QPointF point);

private:
    LinkAnnotation();
    explicit LinkAnnotation(LinkAnnotationPrivate &dd);
    Q_DECLARE_PRIVATE(LinkAnnotation)
    Q_DISABLE_COPY(LinkAnnotation)
};

/**
 * \short Caret annotation.
 *
 * The caret annotation represents a symbol to indicate the presence of text.
 */
class POPPLER_QT6_EXPORT CaretAnnotation : public Annotation
{
    friend class AnnotationPrivate;

public:
    CaretAnnotation();
    ~CaretAnnotation() override;
    SubType subType() const override;

    /**
     * The symbols for the caret annotation.
     */
    enum CaretSymbol
    {
        None,
        P
    };

    CaretSymbol caretSymbol() const;
    void setCaretSymbol(CaretSymbol symbol);

private:
    explicit CaretAnnotation(CaretAnnotationPrivate &dd);
    Q_DECLARE_PRIVATE(CaretAnnotation)
    Q_DISABLE_COPY(CaretAnnotation)
};

/**
 * \short File attachment annotation.
 *
 * The file attachment annotation represents a file embedded in the document.
 */
class POPPLER_QT6_EXPORT FileAttachmentAnnotation : public Annotation
{
    friend class AnnotationPrivate;

public:
    ~FileAttachmentAnnotation() override;
    SubType subType() const override;

    /**
     * Returns the name of the icon of this annotation.
     */
    QString fileIconName() const;
    /**
     * Sets a new name for the icon of this annotation.
     */
    void setFileIconName(const QString &icon);

    /**
     * Returns the EmbeddedFile of this annotation.
     */
    EmbeddedFile *embeddedFile() const;
    /**
     * Sets a new EmbeddedFile for this annotation.
     *
     * \note FileAttachmentAnnotation takes ownership of the object
     */
    void setEmbeddedFile(EmbeddedFile *ef);

private:
    FileAttachmentAnnotation();
    explicit FileAttachmentAnnotation(FileAttachmentAnnotationPrivate &dd);
    Q_DECLARE_PRIVATE(FileAttachmentAnnotation)
    Q_DISABLE_COPY(FileAttachmentAnnotation)
};

/**
 * \short Sound annotation.
 *
 * The sound annotation represents a sound to be played when activated.
 */
class POPPLER_QT6_EXPORT SoundAnnotation : public Annotation
{
    friend class AnnotationPrivate;

public:
    ~SoundAnnotation() override;
    SubType subType() const override;

    /**
     * Returns the name of the icon of this annotation.
     */
    QString soundIconName() const;
    /**
     * Sets a new name for the icon of this annotation.
     */
    void setSoundIconName(const QString &icon);

    /**
     * Returns the SoundObject of this annotation.
     */
    SoundObject *sound() const;
    /**
     * Sets a new SoundObject for this annotation.
     *
     * \note SoundAnnotation takes ownership of the object
     */
    void setSound(SoundObject *s);

private:
    SoundAnnotation();
    explicit SoundAnnotation(SoundAnnotationPrivate &dd);
    Q_DECLARE_PRIVATE(SoundAnnotation)
    Q_DISABLE_COPY(SoundAnnotation)
};

/**
 * \short Movie annotation.
 *
 * The movie annotation represents a movie to be played when activated.
 */
class POPPLER_QT6_EXPORT MovieAnnotation : public Annotation
{
    friend class AnnotationPrivate;

public:
    ~MovieAnnotation() override;
    SubType subType() const override;

    /**
     * Returns the MovieObject of this annotation.
     */
    MovieObject *movie() const;
    /**
     * Sets a new MovieObject for this annotation.
     *
     * \note MovieAnnotation takes ownership of the object
     */
    void setMovie(MovieObject *movie);

    /**
     * Returns the title of the movie of this annotation.
     */
    QString movieTitle() const;
    /**
     * Sets a new title for the movie of this annotation.
     */
    void setMovieTitle(const QString &title);

private:
    MovieAnnotation();
    explicit MovieAnnotation(MovieAnnotationPrivate &dd);
    Q_DECLARE_PRIVATE(MovieAnnotation)
    Q_DISABLE_COPY(MovieAnnotation)
};

/**
 * \short Screen annotation.
 *
 * The screen annotation represents a screen to be played when activated.
 */
class POPPLER_QT6_EXPORT ScreenAnnotation : public Annotation
{
    friend class AnnotationPrivate;

public:
    ~ScreenAnnotation() override;

    SubType subType() const override;

    /**
     * Returns the LinkRendition of this annotation.
     */
    LinkRendition *action() const;

    /**
     * Sets a new LinkRendition for this annotation.
     *
     * \note ScreenAnnotation takes ownership of the object
     */
    void setAction(LinkRendition *action);

    /**
     * Returns the title of the screen of this annotation.
     */
    QString screenTitle() const;

    /**
     * Sets a new title for the screen of this annotation.
     */
    void setScreenTitle(const QString &title);

    /**
     * Returns the additional action of the given @p type for the annotation or
     * @c 0 if no action has been defined.
     */
    std::unique_ptr<Link> additionalAction(AdditionalActionType type) const;

private:
    ScreenAnnotation();
    explicit ScreenAnnotation(ScreenAnnotationPrivate &dd);
    Q_DECLARE_PRIVATE(ScreenAnnotation)
    Q_DISABLE_COPY(ScreenAnnotation)
};

/**
 * \short Widget annotation.
 *
 * The widget annotation represents a widget (form field) on a page.
 *
 * \note This class is just provided for consistency of the annotation API,
 *       use the FormField classes to get all the form-related information.
 */
class POPPLER_QT6_EXPORT WidgetAnnotation : public Annotation
{
    friend class AnnotationPrivate;

public:
    ~WidgetAnnotation() override;

    SubType subType() const override;

    /**
     * Returns the additional action of the given @p type for the annotation or
     * @c 0 if no action has been defined.
     */
    std::unique_ptr<Link> additionalAction(AdditionalActionType type) const;

private:
    WidgetAnnotation();
    explicit WidgetAnnotation(WidgetAnnotationPrivate &dd);
    Q_DECLARE_PRIVATE(WidgetAnnotation)
    Q_DISABLE_COPY(WidgetAnnotation)
};

/**
 * \short RichMedia annotation.
 *
 * The RichMedia annotation represents a video or sound on a page.
 */
class POPPLER_QT6_EXPORT RichMediaAnnotation : public Annotation
{
    friend class AnnotationPrivate;

public:
    ~RichMediaAnnotation() override;

    SubType subType() const override;

    /**
     * The params object of a RichMediaAnnotation::Instance object.
     *
     * The params object provides media specific parameters, to play
     * back the media inside the PDF viewer.
     *
     * At the moment only parameters for flash player are supported.
     */
    class POPPLER_QT6_EXPORT Params
    {
        friend class AnnotationPrivate;

    public:
        Params();
        ~Params();

        /**
         * Returns the parameters for the flash player.
         */
        QString flashVars() const;

    private:
        void setFlashVars(const QString &flashVars);

        class Private;
        QScopedPointer<Private> d;
    };

    /**
     * The instance object of a RichMediaAnnotation::Configuration object.
     *
     * The instance object represents one media object, that should be shown
     * on the page. It has a media type and a Params object, to define the
     * media specific parameters.
     */
    class POPPLER_QT6_EXPORT Instance
    {
        friend class AnnotationPrivate;

    public:
        /**
         * Describes the media type of the instance.
         */
        enum Type
        {
            Type3D, ///< A 3D media file.
            TypeFlash, ///< A Flash media file.
            TypeSound, ///< A sound media file.
            TypeVideo ///< A video media file.
        };

        Instance();
        ~Instance();

        /**
         * Returns the media type of the instance.
         */
        Type type() const;

        /**
         * Returns the params object of the instance or @c 0 if it doesn't exist.
         */
        RichMediaAnnotation::Params *params() const;

    private:
        void setType(Type type);
        void setParams(RichMediaAnnotation::Params *params);

        class Private;
        QScopedPointer<Private> d;
    };

    /**
     * The configuration object of a RichMediaAnnotation::Content object.
     *
     * The configuration object provides access to the various Instance objects
     * of the rich media annotation.
     */
    class POPPLER_QT6_EXPORT Configuration
    {
        friend class AnnotationPrivate;

    public:
        /**
         * Describes the media type of the configuration.
         */
        enum Type
        {
            Type3D, ///< A 3D media file.
            TypeFlash, ///< A Flash media file.
            TypeSound, ///< A sound media file.
            TypeVideo ///< A video media file.
        };

        Configuration();
        ~Configuration();

        /**
         * Returns the media type of the configuration.
         */
        Type type() const;

        /**
         * Returns the name of the configuration.
         */
        QString name() const;

        /**
         * Returns the list of Instance objects of the configuration.
         */
        QList<RichMediaAnnotation::Instance *> instances() const;

    private:
        void setType(Type type);
        void setName(const QString &name);
        void setInstances(const QList<RichMediaAnnotation::Instance *> &instances);

        class Private;
        QScopedPointer<Private> d;
    };

    /**
     * The asset object of a RichMediaAnnotation::Content object.
     *
     * The asset object provides a mapping between identifier name, as
     * used in the flash vars string of RichMediaAnnotation::Params,  and the
     * associated file spec object.
     */
    class POPPLER_QT6_EXPORT Asset
    {
        friend class AnnotationPrivate;

    public:
        Asset();
        ~Asset();

        /**
         * Returns the identifier name of the asset.
         */
        QString name() const;

        /**
         * Returns the embedded file the asset points to.
         */
        EmbeddedFile *embeddedFile() const;

    private:
        void setName(const QString &name);
        void setEmbeddedFile(EmbeddedFile *embeddedFile);

        class Private;
        QScopedPointer<Private> d;
    };

    /**
     * The content object of a RichMediaAnnotation.
     *
     * The content object provides access to the list of configurations
     * and assets of the rich media annotation.
     */
    class POPPLER_QT6_EXPORT Content
    {
        friend class AnnotationPrivate;

    public:
        Content();
        ~Content();

        /**
         * Returns the list of configuration objects of the content object.
         */
        QList<RichMediaAnnotation::Configuration *> configurations() const;

        /**
         * Returns the list of asset objects of the content object.
         */
        QList<RichMediaAnnotation::Asset *> assets() const;

    private:
        void setConfigurations(const QList<RichMediaAnnotation::Configuration *> &configurations);
        void setAssets(const QList<RichMediaAnnotation::Asset *> &assets);

        class Private;
        QScopedPointer<Private> d;
    };

    /**
     * The activation object of the RichMediaAnnotation::Settings object.
     *
     * The activation object is a wrapper around the settings for the activation
     * state. At the moment it provides only the activation condition.
     */
    class POPPLER_QT6_EXPORT Activation
    {
        friend class AnnotationPrivate;

    public:
        /**
         * Describes the condition for activating the rich media.
         */
        enum Condition
        {
            PageOpened, ///< Activate when page is opened.
            PageVisible, ///< Activate when page becomes visible.
            UserAction ///< Activate when user interacts with the annotation.
        };

        Activation();
        ~Activation();

        /**
         * Returns the activation condition.
         */
        Condition condition() const;

    private:
        void setCondition(Condition condition);

        class Private;
        QScopedPointer<Private> d;
    };

    /**
     * The deactivation object of the RichMediaAnnotation::Settings object.
     *
     * The deactivation object is a wrapper around the settings for the deactivation
     * state. At the moment it provides only the deactivation condition.
     */
    class POPPLER_QT6_EXPORT Deactivation
    {
        friend class AnnotationPrivate;

    public:
        /**
         * Describes the condition for deactivating the rich media.
         */
        enum Condition
        {
            PageClosed, ///< Deactivate when page is closed.
            PageInvisible, ///< Deactivate when page becomes invisible.
            UserAction ///< Deactivate when user interacts with the annotation.
        };

        Deactivation();
        ~Deactivation();

        /**
         * Returns the deactivation condition.
         */
        Condition condition() const;

    private:
        void setCondition(Condition condition);

        class Private;
        QScopedPointer<Private> d;
    };

    /**
     * The settings object of a RichMediaAnnotation.
     *
     * The settings object provides access to the configuration objects
     * for annotation activation and deactivation.
     */
    class POPPLER_QT6_EXPORT Settings
    {
        friend class AnnotationPrivate;

    public:
        Settings();
        ~Settings();

        /**
         * Returns the Activation object of the settings object or @c 0 if it doesn't exist.
         */
        RichMediaAnnotation::Activation *activation() const;

        /**
         * Returns the Deactivation object of the settings object or @c 0 if it doesn't exist.
         */
        RichMediaAnnotation::Deactivation *deactivation() const;

    private:
        void setActivation(RichMediaAnnotation::Activation *activation);
        void setDeactivation(RichMediaAnnotation::Deactivation *deactivation);

        class Private;
        QScopedPointer<Private> d;
    };

    /**
     * Returns the Settings object of the rich media annotation or @c 0 if it doesn't exist.
     */
    RichMediaAnnotation::Settings *settings() const;

    /**
     * Returns the Content object of the rich media annotation or @c 0 if it doesn't exist.
     */
    RichMediaAnnotation::Content *content() const;

private:
    void setSettings(RichMediaAnnotation::Settings *settings);
    void setContent(RichMediaAnnotation::Content *content);

    RichMediaAnnotation();
    explicit RichMediaAnnotation(RichMediaAnnotationPrivate &dd);
    Q_DECLARE_PRIVATE(RichMediaAnnotation)
    Q_DISABLE_COPY(RichMediaAnnotation)
};

}

#endif

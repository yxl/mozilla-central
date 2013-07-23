# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

ZXING_JAVA_FILES := \
  zxing/BarcodeFormat.java \
  zxing/Binarizer.java \
  zxing/BinaryBitmap.java \
  zxing/ChecksumException.java \
  zxing/DecodeHintType.java \
  zxing/Dimension.java \
  zxing/EncodeHintType.java \
  zxing/FormatException.java \
  zxing/InvertedLuminanceSource.java \
  zxing/LuminanceSource.java \
  zxing/MultiFormatReader.java \
  zxing/MultiFormatWriter.java \
  zxing/NotFoundException.java \
  zxing/PlanarYUVLuminanceSource.java \
  zxing/Reader.java \
  zxing/ReaderException.java \
  zxing/Result.java \
  zxing/ResultMetadataType.java \
  zxing/ResultPoint.java \
  zxing/ResultPointCallback.java \
  zxing/RGBLuminanceSource.java \
  zxing/Writer.java \
  zxing/WriterException.java \
  zxing/WriterException.java \
  zxing/aztec/AztecDetectorResult.java \
  zxing/aztec/AztecReader.java \
  zxing/aztec/AztecWriter.java \
  zxing/aztec/decoder/Decoder.java \
  zxing/aztec/detector/Detector.java \
  zxing/aztec/encoder/AztecCode.java \
  zxing/aztec/encoder/Encoder.java \
  zxing/client/android/AmbientLightManager.java \
  zxing/client/android/BeepManager.java \
  zxing/client/android/CaptureActivity.java \
  zxing/client/android/CaptureActivityHandler.java \
  zxing/client/android/Contents.java \
  zxing/client/android/DecodeFormatManager.java \
  zxing/client/android/DecodeHandler.java \
  zxing/client/android/DecodeHintManager.java \
  zxing/client/android/DecodeThread.java \
  zxing/client/android/FinishListener.java \
  zxing/client/android/HttpHelper.java \
  zxing/client/android/InactivityTimer.java \
  zxing/client/android/IntentSource.java \
  zxing/client/android/Intents.java \
  zxing/client/android/LocaleManager.java \
  zxing/client/android/ScanFromWebPageManager.java \
  zxing/client/android/ViewfinderResultPointCallback.java \
  zxing/client/android/ViewfinderView.java \
  zxing/client/android/camera/AutoFocusManager.java \
  zxing/client/android/camera/CameraConfigurationManager.java \
  zxing/client/android/camera/CameraManager.java \
  zxing/client/android/camera/FrontLightMode.java \
  zxing/client/android/camera/PreviewCallback.java \
  zxing/client/android/camera/exposure/DefaultExposureInterface.java \
  zxing/client/android/camera/exposure/ExposureInterface.java \
  zxing/client/android/camera/exposure/ExposureManager.java \
  zxing/client/android/camera/exposure/FroyoExposureInterface.java \
  zxing/client/android/camera/open/DefaultOpenCameraInterface.java \
  zxing/client/android/camera/open/GingerbreadOpenCameraInterface.java \
  zxing/client/android/camera/open/OpenCameraInterface.java \
  zxing/client/android/camera/open/OpenCameraManager.java \
  zxing/client/android/common/PlatformSupportManager.java \
  zxing/client/android/common/executor/AsyncTaskExecInterface.java \
  zxing/client/android/common/executor/AsyncTaskExecManager.java \
  zxing/client/android/common/executor/DefaultAsyncTaskExecInterface.java \
  zxing/client/android/common/executor/HoneycombAsyncTaskExecInterface.java \
  zxing/client/android/result/AddressBookResultHandler.java \
  zxing/client/android/result/CalendarResultHandler.java \
  zxing/client/android/result/EmailAddressResultHandler.java \
  zxing/client/android/result/GeoResultHandler.java \
  zxing/client/android/result/ISBNResultHandler.java \
  zxing/client/android/result/ProductResultHandler.java \
  zxing/client/android/result/ResultButtonListener.java \
  zxing/client/android/result/ResultHandler.java \
  zxing/client/android/result/ResultHandlerFactory.java \
  zxing/client/android/result/SMSResultHandler.java \
  zxing/client/android/result/TelResultHandler.java \
  zxing/client/android/result/TextResultHandler.java \
  zxing/client/android/result/URIResultHandler.java \
  zxing/client/android/result/WifiResultHandler.java \
  zxing/client/android/wifi/NetworkType.java \
  zxing/client/android/wifi/WifiConfigManager.java \
  zxing/client/result/AbstractDoCoMoResultParser.java \
  zxing/client/result/AddressBookAUResultParser.java \
  zxing/client/result/AddressBookDoCoMoResultParser.java \
  zxing/client/result/AddressBookParsedResult.java \
  zxing/client/result/BizcardResultParser.java \
  zxing/client/result/BookmarkDoCoMoResultParser.java \
  zxing/client/result/CalendarParsedResult.java \
  zxing/client/result/EmailAddressParsedResult.java \
  zxing/client/result/EmailAddressResultParser.java \
  zxing/client/result/EmailDoCoMoResultParser.java \
  zxing/client/result/ExpandedProductParsedResult.java \
  zxing/client/result/ExpandedProductResultParser.java \
  zxing/client/result/GeoParsedResult.java \
  zxing/client/result/GeoResultParser.java \
  zxing/client/result/ISBNParsedResult.java \
  zxing/client/result/ISBNResultParser.java \
  zxing/client/result/ParsedResult.java \
  zxing/client/result/ParsedResultType.java \
  zxing/client/result/ProductParsedResult.java \
  zxing/client/result/ProductResultParser.java \
  zxing/client/result/ResultParser.java \
  zxing/client/result/SMSMMSResultParser.java \
  zxing/client/result/SMSParsedResult.java \
  zxing/client/result/SMSTOMMSTOResultParser.java \
  zxing/client/result/SMTPResultParser.java \
  zxing/client/result/TelParsedResult.java \
  zxing/client/result/TelResultParser.java \
  zxing/client/result/TextParsedResult.java \
  zxing/client/result/URIParsedResult.java \
  zxing/client/result/URIResultParser.java \
  zxing/client/result/URLTOResultParser.java \
  zxing/client/result/VCardResultParser.java \
  zxing/client/result/VEventResultParser.java \
  zxing/client/result/WifiParsedResult.java \
  zxing/client/result/WifiResultParser.java \
  zxing/common/BitArray.java \
  zxing/common/BitMatrix.java \
  zxing/common/BitSource.java \
  zxing/common/CharacterSetECI.java \
  zxing/common/DecoderResult.java \
  zxing/common/DefaultGridSampler.java \
  zxing/common/DetectorResult.java \
  zxing/common/GlobalHistogramBinarizer.java \
  zxing/common/GridSampler.java \
  zxing/common/HybridBinarizer.java \
  zxing/common/PerspectiveTransform.java \
  zxing/common/StringUtils.java \
  zxing/common/detector/MathUtils.java \
  zxing/common/detector/MonochromeRectangleDetector.java \
  zxing/common/detector/WhiteRectangleDetector.java \
  zxing/common/reedsolomon/GenericGF.java \
  zxing/common/reedsolomon/GenericGFPoly.java \
  zxing/common/reedsolomon/ReedSolomonDecoder.java \
  zxing/common/reedsolomon/ReedSolomonEncoder.java \
  zxing/common/reedsolomon/ReedSolomonException.java \
  zxing/datamatrix/DataMatrixReader.java \
  zxing/datamatrix/DataMatrixWriter.java \
  zxing/datamatrix/decoder/BitMatrixParser.java \
  zxing/datamatrix/decoder/DataBlock.java \
  zxing/datamatrix/decoder/DecodedBitStreamParser.java \
  zxing/datamatrix/decoder/Decoder.java \
  zxing/datamatrix/decoder/Version.java \
  zxing/datamatrix/detector/Detector.java \
  zxing/datamatrix/encoder/ASCIIEncoder.java \
  zxing/datamatrix/encoder/Base256Encoder.java \
  zxing/datamatrix/encoder/C40Encoder.java \
  zxing/datamatrix/encoder/DataMatrixSymbolInfo144.java \
  zxing/datamatrix/encoder/DefaultPlacement.java \
  zxing/datamatrix/encoder/EdifactEncoder.java \
  zxing/datamatrix/encoder/Encoder.java \
  zxing/datamatrix/encoder/EncoderContext.java \
  zxing/datamatrix/encoder/ErrorCorrection.java \
  zxing/datamatrix/encoder/HighLevelEncoder.java \
  zxing/datamatrix/encoder/SymbolInfo.java \
  zxing/datamatrix/encoder/SymbolShapeHint.java \
  zxing/datamatrix/encoder/TextEncoder.java \
  zxing/datamatrix/encoder/X12Encoder.java \
  zxing/maxicode/MaxiCodeReader.java \
  zxing/maxicode/decoder/BitMatrixParser.java \
  zxing/maxicode/decoder/DecodedBitStreamParser.java \
  zxing/maxicode/decoder/Decoder.java \
  zxing/multi/ByQuadrantReader.java \
  zxing/multi/GenericMultipleBarcodeReader.java \
  zxing/multi/MultipleBarcodeReader.java \
  zxing/multi/qrcode/QRCodeMultiReader.java \
  zxing/multi/qrcode/detector/MultiDetector.java \
  zxing/multi/qrcode/detector/MultiFinderPatternFinder.java \
  zxing/oned/CodaBarReader.java \
  zxing/oned/CodaBarWriter.java \
  zxing/oned/Code128Reader.java \
  zxing/oned/Code128Writer.java \
  zxing/oned/Code39Reader.java \
  zxing/oned/Code39Writer.java \
  zxing/oned/Code93Reader.java \
  zxing/oned/EAN13Reader.java \
  zxing/oned/EAN13Writer.java \
  zxing/oned/EAN8Reader.java \
  zxing/oned/EAN8Writer.java \
  zxing/oned/EANManufacturerOrgSupport.java \
  zxing/oned/ITFReader.java \
  zxing/oned/ITFWriter.java \
  zxing/oned/MultiFormatOneDReader.java \
  zxing/oned/MultiFormatUPCEANReader.java \
  zxing/oned/OneDReader.java \
  zxing/oned/OneDimensionalCodeWriter.java \
  zxing/oned/UPCAReader.java \
  zxing/oned/UPCAWriter.java \
  zxing/oned/UPCEANExtension2Support.java \
  zxing/oned/UPCEANExtension5Support.java \
  zxing/oned/UPCEANExtensionSupport.java \
  zxing/oned/UPCEANReader.java \
  zxing/oned/UPCEANWriter.java \
  zxing/oned/UPCEReader.java \
  zxing/oned/rss/AbstractRSSReader.java \
  zxing/oned/rss/DataCharacter.java \
  zxing/oned/rss/FinderPattern.java \
  zxing/oned/rss/Pair.java \
  zxing/oned/rss/RSS14Reader.java \
  zxing/oned/rss/RSSUtils.java \
  zxing/oned/rss/expanded/BitArrayBuilder.java \
  zxing/oned/rss/expanded/ExpandedPair.java \
  zxing/oned/rss/expanded/ExpandedRow.java \
  zxing/oned/rss/expanded/RSSExpandedReader.java \
  zxing/oned/rss/expanded/decoders/AI013103decoder.java \
  zxing/oned/rss/expanded/decoders/AI01320xDecoder.java \
  zxing/oned/rss/expanded/decoders/AI01392xDecoder.java \
  zxing/oned/rss/expanded/decoders/AI01393xDecoder.java \
  zxing/oned/rss/expanded/decoders/AI013x0x1xDecoder.java \
  zxing/oned/rss/expanded/decoders/AI013x0xDecoder.java \
  zxing/oned/rss/expanded/decoders/AI01AndOtherAIs.java \
  zxing/oned/rss/expanded/decoders/AI01decoder.java \
  zxing/oned/rss/expanded/decoders/AI01weightDecoder.java \
  zxing/oned/rss/expanded/decoders/AbstractExpandedDecoder.java \
  zxing/oned/rss/expanded/decoders/AnyAIDecoder.java \
  zxing/oned/rss/expanded/decoders/BlockParsedResult.java \
  zxing/oned/rss/expanded/decoders/CurrentParsingState.java \
  zxing/oned/rss/expanded/decoders/DecodedChar.java \
  zxing/oned/rss/expanded/decoders/DecodedInformation.java \
  zxing/oned/rss/expanded/decoders/DecodedNumeric.java \
  zxing/oned/rss/expanded/decoders/DecodedObject.java \
  zxing/oned/rss/expanded/decoders/FieldParser.java \
  zxing/oned/rss/expanded/decoders/GeneralAppIdDecoder.java \
  zxing/pdf417/PDF417Common.java \
  zxing/pdf417/PDF417Reader.java \
  zxing/pdf417/PDF417ResultMetadata.java \
  zxing/pdf417/PDF417Writer.java \
  zxing/pdf417/decoder/BarcodeMetadata.java \
  zxing/pdf417/decoder/BarcodeValue.java \
  zxing/pdf417/decoder/BoundingBox.java \
  zxing/pdf417/decoder/Codeword.java \
  zxing/pdf417/decoder/DecodedBitStreamParser.java \
  zxing/pdf417/decoder/DetectionResult.java \
  zxing/pdf417/decoder/DetectionResultColumn.java \
  zxing/pdf417/decoder/DetectionResultRowIndicatorColumn.java \
  zxing/pdf417/decoder/PDF417CodewordDecoder.java \
  zxing/pdf417/decoder/PDF417ScanningDecoder.java \
  zxing/pdf417/decoder/ec/ErrorCorrection.java \
  zxing/pdf417/decoder/ec/ModulusGF.java \
  zxing/pdf417/decoder/ec/ModulusPoly.java \
  zxing/pdf417/detector/Detector.java \
  zxing/pdf417/detector/PDF417DetectorResult.java \
  zxing/pdf417/encoder/BarcodeMatrix.java \
  zxing/pdf417/encoder/BarcodeRow.java \
  zxing/pdf417/encoder/Compaction.java \
  zxing/pdf417/encoder/Dimensions.java \
  zxing/pdf417/encoder/PDF417.java \
  zxing/pdf417/encoder/PDF417ErrorCorrection.java \
  zxing/pdf417/encoder/PDF417HighLevelEncoder.java \
  zxing/qrcode/QRCodeReader.java \
  zxing/qrcode/QRCodeWriter.java \
  zxing/qrcode/decoder/BitMatrixParser.java \
  zxing/qrcode/decoder/DataBlock.java \
  zxing/qrcode/decoder/DataMask.java \
  zxing/qrcode/decoder/DecodedBitStreamParser.java \
  zxing/qrcode/decoder/Decoder.java \
  zxing/qrcode/decoder/ErrorCorrectionLevel.java \
  zxing/qrcode/decoder/FormatInformation.java \
  zxing/qrcode/decoder/Mode.java \
  zxing/qrcode/decoder/Version.java \
  zxing/qrcode/detector/AlignmentPattern.java \
  zxing/qrcode/detector/AlignmentPatternFinder.java \
  zxing/qrcode/detector/Detector.java \
  zxing/qrcode/detector/FinderPattern.java \
  zxing/qrcode/detector/FinderPatternFinder.java \
  zxing/qrcode/detector/FinderPatternInfo.java \
  zxing/qrcode/encoder/BlockPair.java \
  zxing/qrcode/encoder/ByteMatrix.java \
  zxing/qrcode/encoder/Encoder.java \
  zxing/qrcode/encoder/MaskUtil.java \
  zxing/qrcode/encoder/MatrixUtil.java \
  zxing/qrcode/encoder/QRCode.java \
  $(NULL)

ZXING_RES_LAYOUT := \
  res/layout/capture.xml \
  $(NULL)

ZXING_RES_VALUES := \
  res/values/ids.xml \
  $(NULL)


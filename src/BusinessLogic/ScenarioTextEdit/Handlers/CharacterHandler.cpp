#include "CharacterHandler.h"

#include <BusinessLogic/ScenarioTextEdit/ScenarioTextEdit.h>

#include <Domain/Character.h>

#include <Storage/StorageFacade.h>
#include <Storage/CharacterStorage.h>

#include <QKeyEvent>
#include <QTextBlock>

using namespace KeyProcessingLayer;
using namespace StorageLayer;


CharacterHandler::CharacterHandler(ScenarioTextEdit* _editor) :
	StandardKeyHandler(_editor)
{
}

void CharacterHandler::handleEnter()
{
	//
	// Получим необходимые значения
	//
	// ... курсор в текущем положении
	QTextCursor cursor = editor()->textCursor();
	// ... блок текста в котором находится курсор
	QTextBlock currentBlock = cursor.block();
	// ... текст до курсора
	QString cursorBackwardText = currentBlock.text().left(cursor.positionInBlock());
	// ... текст после курсора
	QString cursorForwardText = currentBlock.text().mid(cursor.positionInBlock());


	//
	// Обработка
	//
	if (editor()->isCompleterVisible()) {
		//! Если открыт подстановщик

		//
		// Вставить выбранный вариант
		//
		editor()->applyCompletion();
	} else {
		//! Подстановщик закрыт

		if (cursor.hasSelection()) {
			//! Есть выделение

			//
			// Ни чего не делаем
			//
		} else {
			//! Нет выделения

			if (cursorBackwardText.isEmpty()
				&& cursorForwardText.isEmpty()) {
				//! Текст пуст

				//
				// Ни чего не делаем
				//
			} else {
				//! Текст не пуст

				if (cursorBackwardText.isEmpty()) {
					//! В начале блока

					//
					// Вставим блок имени героя перед собой
					//
					cursor.insertBlock();
					editor()->setScenarioBlockType(ScenarioTextBlockStyle::Character);
				} else if (cursorForwardText.isEmpty()) {
					//! В конце блока

					//
					// Вставить блок реплики героя
					//
					cursor.insertBlock();
					editor()->setScenarioBlockType(ScenarioTextBlockStyle::Dialog);
				} else {
					//! Внутри блока

					//
					// Ни чего не делаем
					//
				}
			}
		}
	}
}

void CharacterHandler::handleTab()
{
	//
	// Получим необходимые значения
	//
	// ... курсор в текущем положении
	QTextCursor cursor = editor()->textCursor();
	// ... блок текста в котором находится курсор
	QTextBlock currentBlock = cursor.block();
	// ... текст до курсора
	QString cursorBackwardText = currentBlock.text().left(cursor.positionInBlock());
	// ... текст после курсора
	QString cursorForwardText = currentBlock.text().mid(cursor.positionInBlock());


	//
	// Обработка
	//
	if (editor()->isCompleterVisible()) {
		//! Если открыт подстановщик

		//
		// Работаем, как ENTER
		//
		handleEnter();
	} else {
		//! Подстановщик закрыт

		if (cursor.hasSelection()) {
			//! Есть выделение

			//
			// Ни чего не делаем
			//
		} else {
			//! Нет выделения

			if (cursorBackwardText.isEmpty()
				&& cursorForwardText.isEmpty()) {
				//! Текст пуст

				//
				// Cменить стиль на описание действия
				//
				editor()->setScenarioBlockType(ScenarioTextBlockStyle::Action);
			} else {
				//! Текст не пуст

				if (cursorBackwardText.isEmpty()) {
					//! В начале блока

					//
					// Ни чего не делаем
					//
				} else if (cursorForwardText.isEmpty()) {
					//! В конце блока

					//
					// Вставить блок ремарки
					//
					cursor.insertBlock();
					editor()->setScenarioBlockType(ScenarioTextBlockStyle::Parenthetical);
				} else {
					//! Внутри блока

					//
					// Ни чего не делаем
					//
				}
			}
		}
	}
}

void CharacterHandler::handleOther()
{
	//
	// Получим необходимые значения
	//
	// ... курсор в текущем положении
	QTextCursor cursor = editor()->textCursor();
	// ... блок текста в котором находится курсор
	QTextBlock currentBlock = cursor.block();
	// ... текст блока
	QString currentBlockText = currentBlock.text();


	//
	// Получим модель подсказок для текущей секции и выведем пользователю
	//
	QAbstractItemModel* characterModel = StorageFacade::characterStorage()->all();

	//
	// Дополним текст
	//
	editor()->complete(characterModel, currentBlockText);
}

#include "GlobalVariables.h"
#include "ImGuiManager.h"
#include <assert.h>
#include <json.hpp>
#include <fstream>
#include <Windows.h>

using namespace nlohmann;

namespace DaiEngine {
	GlobalVariables* GlobalVariables::GetInstance() {
		static GlobalVariables instance;

		return &instance;
	}

	void GlobalVariables::CreateGroup(const std::string& groupName) {
		//指定名のオブジェクトがなければ追加する
		datas_[groupName];
	}

	void GlobalVariables::Update() {
#ifdef USE_IMGUI
		ImGui::Begin("Global Variables");

		ImGui::BeginChild("GroupList", ImVec2(200, 0), true); // ← 左のグループ一覧
		for (auto& [groupName, group] : datas_) {
			if (ImGui::Selectable(groupName.c_str(), selectedGroup == groupName)) {
				selectedGroup = groupName;
			}
		}
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("GroupContent", ImVec2(0, 0), true); // ← 右の内容
		if (!selectedGroup.empty()) {
			Group& group = datas_.at(selectedGroup);
			for (auto& [itemName, item] : group.items) {
				if (std::holds_alternative<int32_t>(item.value)) {
					ImGui::InputInt(itemName.c_str(), std::get_if<int32_t>(&item.value));
				}
				else if (std::holds_alternative<float>(item.value)) {
					ImGui::DragFloat(itemName.c_str(), std::get_if<float>(&item.value), 0.01f);
				}
				else if (std::holds_alternative<Vector2>(item.value)) {
					ImGui::DragFloat2(itemName.c_str(), reinterpret_cast<float*>(std::get_if<Vector2>(&item.value)), 0.1f);
				}
				else if (std::holds_alternative<Vector3>(item.value)) {
					ImGui::DragFloat3(itemName.c_str(), reinterpret_cast<float*>(std::get_if<Vector3>(&item.value)), 0.01f);
				}
			}

			if (ImGui::Button("Save")) {
				SaveFile(selectedGroup);
				std::string msg = std::format("{}.json saved.", selectedGroup);
				MessageBoxA(nullptr, msg.c_str(), "GlobalVariables", 0);
			}
		}
		ImGui::EndChild();

		ImGui::End();

#endif // _DEBUG
	}

	void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, int32_t value) {
		//グループの参照を取得
		Group& group = datas_[groupName];
		//新しい項目のデータを設定
		Item newItem{};
		newItem.value = value;
		//設定した項目をstd::mapに追加
		group.items[key] = newItem;

	}

	void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, float value) {
		// グループの参照を取得
		Group& group = datas_[groupName];
		// 新しい項目のデータを設定
		Item newItem{};
		newItem.value = value;
		// 設定した項目をstd::mapに追加
		group.items[key] = newItem;

	}

	void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, const Vector3& value) {
		// グループの参照を取得
		Group& group = datas_[groupName];
		// 新しい項目のデータを設定
		Item newItem{};
		newItem.value = value;
		// 設定した項目をstd::mapに追加
		group.items[key] = newItem;

	}

	void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, const Vector2& value) {
		// グループの参照を取得
		Group& group = datas_[groupName];
		// 新しい項目のデータを設定
		Item newItem{};
		newItem.value = value;
		// 設定した項目をstd::mapに追加
		group.items[key] = newItem;

	}

	void GlobalVariables::SaveFile(const std::string& groupName) {

		//グループを検索
		std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);
		//未登録チェック
		assert(itGroup != datas_.end());

		json root;

		root = json::object();
		//jsonオブジェクト登録
		root[groupName] = json::object();
		//各項目について
		for (std::map<std::string, Item>::iterator itItem = itGroup->second.items.begin();
			itItem != itGroup->second.items.end(); ++itItem) {
			//項目名を取得
			const std::string& itemName = itItem->first;
			//項目の参照を取得
			Item& item = itItem->second;

			//int32_t型の値を保持していれば
			if (std::holds_alternative<int32_t>(item.value)) {
				//int32_t型の値を登録
				root[groupName][itemName] = std::get<int32_t>(item.value);
				//float型の値を保持していれば
			}
			else if (std::holds_alternative<float>(item.value)) {
				//float型の値を登録
				root[groupName][itemName] = std::get<float>(item.value);
				//Vec3型の値を保持していれば
			}
			else if (std::holds_alternative<Vector3>(item.value)) {
				//Vec3型の値を登録
				Vector3 value = std::get<Vector3>(item.value);
				root[groupName][itemName] = json::array({ value.x, value.y, value.z });
			}
			else if (std::holds_alternative<Vector2>(item.value)) {
				//Vec2型の値を保持
				Vector2 value = std::get<Vector2>(item.value);
				root[groupName][itemName] = json::array({ value.x,value.y });
			}
		}

		//ディレクトリがなければ作成する
		std::filesystem::path dir(kDirectoryPath);
		if (!std::filesystem::exists(kDirectoryPath)) {
			std::filesystem::create_directory(kDirectoryPath);
		}
		//書き込むJSONファイルのフルパスを合成する
		std::string filePath = kDirectoryPath + groupName + ".json";
		//書き込む用のファイルストリーム
		std::ofstream ofs;
		//ファイルを書き込み用に開く
		ofs.open(filePath);

		//ファイルオープン失敗？
		if (ofs.fail()) {
			std::string message = "Failed open data file for write.";
			MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
			assert(0);
			return;
		}

		//ファイルにjsonの文字列を書き込む(インデント幅4)
		ofs << std::setw(4) << root << std::endl;
		//ファイルを閉じる
		ofs.close();

	}

	void GlobalVariables::LoadFiles() {

		std::string DirectoryPath = "Resources/GlobalVariables/";
		//ディレクトリがなければスキップする
		if (!std::filesystem::exists(DirectoryPath)) {
			return;
		}

		std::filesystem::directory_iterator dir_it(DirectoryPath);
		for (const std::filesystem::directory_entry& entry : dir_it) {

			//ファイルパスを取得
			const std::filesystem::path& filePath = entry.path();

			//ファイル拡張子を取得
			std::string extension = filePath.extension().string();
			//.jsonファイル以外はスキップ
			if (extension.compare(".json") != 0) {
				continue;
			}

			LoadFile(filePath.stem().string());
		}
	}

	void GlobalVariables::LoadFile(const std::string& groupName) {
		//読み込むJSONファイルのフルパスを合成する
		std::string filePath = kDirectoryPath + groupName + ".json";
		//読み込み用ファイルストリーム
		std::ifstream ifs;
		//ファイルを読み込み用に開く
		ifs.open(filePath);

		//ファイルオープン失敗？
		if (ifs.fail()) {
			std::string message = "Failed open data file for write.";
			MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
			assert(0);
			return;
		}

		json root;

		//json文字列からjsonのデータ構造に展開
		ifs >> root;
		//ファイルを閉じる
		ifs.close();

		//グループを検索
		json::iterator itGroup = root.find(groupName);

		//未登録チェック
		assert(itGroup != root.end());

		//各アイテムにつぃて
		for (json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem) {
			//アイテム名の取得
			const std::string& itemName = itItem.key();

			//int32_t型の値を保持していれば
			if (itItem->is_number_integer()) {
				//int32_t型の値を登録
				int32_t value = itItem->get<int32_t>();
				SetValue(groupName, itemName, value);
				//float型の値を保持していれば
			}
			else if (itItem->is_number_float()) {
				//float型の値を登録
				double value = itItem->get<double>();
				SetValue(groupName, itemName, static_cast<float>(value));
				//要素数3の配列であれば
			}
			else if (itItem->is_array() && itItem->size() == 3) {
				//float型のjson配列登録
				Vector3 value = { itItem->at(0), itItem->at(1), itItem->at(2) };
				SetValue(groupName, itemName, value);
				//要素数2の配列であれば
			}
			else if (itItem->is_array() && itItem->size() == 2) {
				//float型のjson配列登録
				Vector2 value = { itItem->at(0), itItem->at(1) };
				SetValue(groupName, itemName, value);
			}

		}

	}

	void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, int32_t value) {

		auto group = datas_.find(groupName);

		if (group == datas_.end()) {
			return;
		}

		if (group->second.items.find(key) == group->second.items.end()) {
			SetValue(groupName, key, value);
		}
	}

	void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, float value) {
		auto group = datas_.find(groupName);

		if (group == datas_.end()) {
			return;
		}

		if (group->second.items.find(key) == group->second.items.end()) {
			SetValue(groupName, key, value);
		}
	}

	void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, const Vector3& value) {
		auto group = datas_.find(groupName);

		if (group == datas_.end()) {
			return;
		}

		if (group->second.items.find(key) == group->second.items.end()) {
			SetValue(groupName, key, value);
		}
	}

	void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, const Vector2& value) {
		auto group = datas_.find(groupName);

		if (group == datas_.end()) {
			return;
		}

		if (group->second.items.find(key) == group->second.items.end()) {
			SetValue(groupName, key, value);
		}
	}

	int32_t GlobalVariables::GetIntValue(const std::string& groupName, const std::string& key) const {


		assert(datas_.find(groupName) != datas_.end());

		const Group& group = datas_.at(groupName);

		assert(group.items.find(key) != group.items.end());

		return std::get<int>(group.items.find(key)->second.value);

	}

	float GlobalVariables::GetFloatValue(const std::string& groupName, const std::string& key) const {

		assert(datas_.find(groupName) != datas_.end());

		const Group& group = datas_.at(groupName);

		assert(group.items.find(key) != group.items.end());

		return std::get<float>(group.items.find(key)->second.value);
	}

	Vector3 GlobalVariables::GetVec3Value(const std::string& groupName, const std::string& key) const {

		assert(datas_.find(groupName) != datas_.end());

		const Group& group = datas_.at(groupName);

		assert(group.items.find(key) != group.items.end());

		return std::get<Vector3>(group.items.find(key)->second.value);
	}

	Vector2 GlobalVariables::GetVec2Value(const std::string& groupName, const std::string& key) const {
		assert(datas_.find(groupName) != datas_.end());

		const Group& group = datas_.at(groupName);

		assert(group.items.find(key) != group.items.end());

		return std::get<Vector2>(group.items.find(key)->second.value);
	}
}

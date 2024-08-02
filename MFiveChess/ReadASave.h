#pragma once
#include <iostream>
#include <vector>
#include <fstream> // 用于文件操作
#include <string>
#include "ChessBoradPreserve.h"
/// <summary>
/// 将棋盘信息写入文件
/// </summary>
/// <param name="path">文件路径</param>
/// <param name="board">棋盘</param>
void Write(std::string path, ChessBoardPreserve* board);
/// <summary>
/// 判断文件是否存在
/// </summary>
/// <param name="path">文件路径</param>
bool FileExists(const std::string& path);
/// <summary>
/// 创建数据头(数据名加#)
/// </summary>
/// <param name="string">数据名(std::string)</param>
/// <returns>数据头</returns>
std::string write_tittle(std::string string);
/// <summary>
/// 创建数据头(数据名加#)
/// </summary>
/// <param name="string">数据名(int)</param>
/// <returns>数据头</returns>
std::string write_tittle(int string);
/// <summary>
/// 创建数据内容(数据加:|)
/// </summary>
/// <param name="string">数据(std::string)</param>
/// <returns>数据内容</returns>
std::string write_content(std::string string);
/// <summary>
/// 创建数据内容(数据加:|)
/// </summary>
/// <param name="string">数据(int)</param>
/// <returns>数据内容</returns>
std::string write_content(int string);
/// <summary>
/// 从文件中读取棋盘
/// </summary>
/// <param name="path">文件路径</param>
/// <param name="boardPreserve">保存棋盘的变量</param>
void ReadFromFile(std::string path, ChessBoardPreserve* boardPreserve);

